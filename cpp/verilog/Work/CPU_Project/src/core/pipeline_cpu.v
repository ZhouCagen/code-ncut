`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module pipeline_cpu(
    input wire clk,
    input wire rst_n
);

    // 一、取址 （IF）

    wire [31:0] pc_next;                // 计算出的下一个PC值，用于更新程序计数器
    wire [31:0] pc_plus_4_if;           // 当前PC值加4，指向顺序执行的下一条指令
    wire [31:0] pc_current;             // 当前程序计数器的值，用于读取指令
    wire [31:0] instruction_if;         // 从指令存储器读取的指令

    // 来自冒险检测单元的暂停信号
    wire pc_write_en;                   // PC写使能信号，当为1时允许PC更新，否则暂停PC（即冒险发生时）
    wire if_id_write_en;                // IF/ID流水线寄存器写使能信号，当为1时允许更新寄存器，否则暂停
    wire hazard_stall;                  // 冒险暂停信号，等价于!pc_write_en

    // 来自ID阶段的分支/跳转决定
    wire branch_taken_id;               // 分支指令被采取（为1时）
    wire [31:0] branch_target_addr_id;  // 分支目标地址
    wire jump_id;                       // 跳转指令有效（为1时）
    wire [31:0] jump_target_addr_id;    // 跳转目标地址
    wire jr_id;                         // 寄存器跳转指令有效
    wire [31:0] jr_target_addr_id;      // 寄存器跳转目标地址

    assign pc_plus_4_if = pc_current + 4;
    assign pc_next = (jr_id) ? jr_target_addr_id : ((jump_id) ? jump_target_addr_id : ((branch_taken_id) ? branch_target_addr_id : pc_plus_4_if));
    
        // 使用assign语句计算pc_next，优先级从高到低为： 
        // 如果jr_id为1，则下一个PC是jr_target_addr_id（寄存器跳转）。
        // 否则，如果jump_id为1，则下一个PC是jump_target_addr_id（直接跳转）。
        // 否则，如果branch_taken_id为1，则下一个PC是branch_target_addr_id（分支跳转）。
        // 否则，下一个PC是pc_plus_4_if（顺序执行）。
        // 这种优先级确保了寄存器跳转（jr）具有最高优先级，然后是跳转（jump），最后是分支（branch）。

    // 程序计数器 (PC)
    program_counter pc_instance(
        .clk(clk),
        .rst_n(rst_n),
        .stall(~pc_write_en),
        .branch_taken(branch_taken_id),
        .jump(jump_id),
        .jump_reg(jr_id),
        .branch_addr(branch_target_addr_id),
        .jump_addr(jump_target_addr_id),
        .jump_reg_addr(jr_target_addr_id),
        .pc(pc_current)
    );
    /*
    module program_counter(
        input wire clk,
        input wire rst,
        input wire stall,                   // 流水线暂停信号
        input wire branch_taken,            // 分支跳转信号
        input wire jump,                    // 跳转信号
        input wire jump_reg,                // 寄存器跳转信号
        input wire [31:0] branch_addr,
        input wire [31:0] jump_addr,
        input wire [31:0] jump_reg_addr,

        output reg [31:0] pc                // 程序计数器输出
    );
    */


    // 指令存储器 (Instruction Memory)
    instruction_memory imem_instance(
        .addr(pc_current),
        .instruction(instruction_if)
    );

    // IF/ID 流水线寄存器
    reg [31:0] if_id_pc_plus_4;         // if_id_pc_plus_4：用于存储取指阶段计算出的下一条顺序指令的地址（PC + 4）
    reg [31:0] if_id_instruction;       // if_id_instruction：用于存储从指令内存中取出的当前指令
    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            if_id_pc_plus_4 <= 32'b0;
            if_id_instruction <= 32'b0;
        end else if(if_id_write_en) begin
            if_id_pc_plus_4 <= pc_plus_4_if;  // 将IF阶段计算出的 PC+4 值锁存到寄存器中。
            if_id_instruction <= instruction_if; 
        end
    end

    // 二、译码（ID）
    
    // 控制器 (Control Unit)
    
    /*
    module control_unit(
        input wire [5:0] opcode,             // 指令操作码
        input wire [5:0] funct,              // 功能字段（用于R型指令）
        output reg reg_write_en,             // 寄存器写使能
        output reg mem_to_reg_sel,           // 内存到寄存器选择
        output reg mem_write_en,             // 内存写使能
        output reg mem_read_en,              // 内存读使能
        output reg branch_en,                // 分支指令使能
        output reg alu_src_sel,              // ALU源选择（寄存器/立即数）
        output reg reg_dst_sel,              // 寄存器目标选择
        output reg [1:0] alu_op,             // ALU操作类型指示
        output reg jump_en,                  // 跳转指令使能
        output reg jal_sel,                  // JAL指令选择
        output reg jr_sel                    // JR指令选择
    );
    */

    wire [31:0] instruction_id = if_id_instruction;
    wire [31:0] pc_plus_4_id = if_id_pc_plus_4;

    wire reg_write_en_id;
    wire mem_to_reg_sel_id;
    wire mem_write_en_id;
    wire mem_read_en_id;
    wire branch_en_id;
    wire alu_src_sel_id;
    wire reg_dst_sel_id;
    wire [1:0] alu_op_id;
    wire jump_en_id;
    wire jal_sel_id;
    wire jr_sel_id;

    control_unit ctrl_unit_instance(
        .opcode(instruction_id[31:26]),
        .funct(instruction_id[5:0]),
        .reg_write_en(reg_write_en_id),
        .mem_to_reg_sel(mem_to_reg_sel_id),
        .mem_write_en(mem_write_en_id),
        .mem_read_en(mem_read_en_id),
        .branch_en(branch_en_id),
        .alu_src_sel(alu_src_sel_id),
        .reg_dst_sel(reg_dst_sel_id),
        .alu_op(alu_op_id),
        .jump_en(jump_en_id),
        .jal_sel(jal_sel_id),
        .jr_sel(jr_sel_id)
    );

    // 寄存器文件 (Register File)

    /*
    module register_file(
    input wire clk,
    input wire rst_n,
    input wire write_enable,
    input wire [4:0] read_addr1,
    input wire [4:0] read_addr2,
    input wire [4:0] write_addr,
    input wire [31:0] write_data,
    output wire [31:0] read_data1,
    output wire [31:0] read_data2
    );
    */

    wire write_enable_wb;
    wire [4:0] read_addr1_id = instruction_id[25:21];
    wire [4:0] read_addr2_id = instruction_id[20:16];
    wire [4:0] write_addr_wb;
    wire [31:0] write_data_wb;

    wire [31:0] read_data1_id;
    wire [31:0] read_data2_id;

    register_file reg_file_instance(
        .clk(clk),
        .rst_n(rst_n),
        .write_enable(reg_write_en_wb),
        .read_addr1(read_addr1_id),
        .read_addr2(read_addr2_id),
        .write_addr(write_addr_wb),
        .write_data(write_data_wb),
        .read_data1(read_data1_id),
        .read_data2(read_data2_id)
    );

    // 立即数扩展 (Sign Extend)

    /*
    module sign_extend(
    input wire [15:0] data_in,
    output wire [31:0] data_out
    );
    */

    wire [31:0] sign_extended_imm_id;
    sign_extend signed_extend_instance(
        .data_in(instruction_id[15:0]),
        .data_out(sign_extended_imm_id)
    );

    // 分支逻辑 (Branch Logic)  

    wire [31:0] branch_offset_shifted_id = {sign_extended_imm_id[29:0], 2'b00};
    // 移位处理后的、用于分支指令的地址偏移量
    // 分支偏移量是以字为单位的，而地址是以字节为单位的，所以需要乘以4（左移2位）
    assign branch_target_addr_id = pc_plus_4_id + branch_offset_shifted_id;
    // 添加分支类型判断
    wire is_beq = (instruction_id[31:26] == `OP_BEQ);
    wire is_bne = (instruction_id[31:26] == `OP_BNE);
    // 判断分支是否发生
    wire branch_condition_met_id = (is_beq & (read_data1_id == read_data2_id)) | (is_bne & (read_data1_id != read_data2_id));
    assign branch_taken_id = branch_en_id & branch_condition_met_id;

    // 跳转逻辑（Jump Logic）

    /* 
    +--------+------------------------------+
    |  opcode|          target               |      
    +--------+------------------------------+
    |  6 bits|          26 bits              |
    +--------+------------------------------+
    | 31   26|25                           0 |
    但是 CPU 的 PC 寄存器是 32 位的，而 target 只有 26 位，不能直接当作完整地址
    MIPS 的设计是：
    把这 26 位左移 2 位（因为指令对齐到 4 字节，低 2 位永远是 `00`）
    把当前 PC+4 的高 4 位拼接到前面，补齐成 32 位地址。 这样保证跳转只能落在 当前 256MB 的"区域"内
    */
    assign jump_target_addr_id = {pc_plus_4_id[31:28],instruction_id[25:0], 2'b0};
    assign jump_id = jump_en_id;
    assign jr_id = jr_sel_id;
    assign jr_target_addr_id = read_data1_id; // JR指令的地址来自rs寄存器
    // 目标寄存器选择 (JAL指令使用$ra寄存器)   R-type指令写入rd I-type指令写入rt
    wire [4:0] write_reg_id  = (jal_sel_id) ? 5'd31 : (reg_dst_sel_id) ? instruction_id[15:11] : instruction_id[20:16];

    // 冒险检测单元 (Hazard Detection Unit)
    wire [4:0] ex_rd;
    wire ex_mem_read;
    /*
    module hazard_detection_unit(
    input wire [4:0] id_rs,
    input wire [4:0] id_rt,
    input wire [4:0] ex_rd,
    input wire ex_mem_read,         // EX阶段是否是load指令（内存读取）
    output reg pc_write_en,         // PC写使能（1=允许更新，0=暂停）
    output reg if_id_write_en,      // IF/ID寄存器写使能（1=允许更新，0=暂停）
    output reg control_mux_sel      // 控制信号选择（1=插入空操作，0=正常） 
    );
    */
    hazard_detection_unit hud_instance(
        .id_rs(instruction_id[25:21]),
        .id_rt(instruction_id[20:16]),
        .ex_rd(ex_rd),
        .ex_mem_read(ex_mem_read),
        .pc_write_en(pc_write_en),
        .if_id_write_en(if_id_write_en),
        .control_mux_sel(hazard_stall) // 通常用于清零ID/EX的控制信号
    );

    // ID/EX 流水线寄存器
    // 该模块用于在指令译码（ID）阶段和执行（EX）阶段之间暂存数据和控制信号
    reg [31:0] id_ex_pc_plus_4;          // 存储PC+4值，用于后续计算跳转地址或异常处理
    reg [31:0] id_ex_read_data1;         // 从寄存器文件读取的第一个操作数（rs值）
    reg [31:0] id_ex_read_data2;         // 从寄存器文件读取的第二个操作数（rt值）
    reg [31:0] id_ex_sign_extended_imm;  // 经符号扩展后的立即数
    reg [4:0] id_ex_rs;                  // rs寄存器编号（用于转发检测）
    reg [4:0] id_ex_rt;                  // rt寄存器编号（用于转发检测）
    reg [4:0] id_ex_write_reg;           // 目标寄存器编号（可能为rd或rt）
    reg id_ex_reg_write_en;              // 寄存器写使能信号（控制是否写回寄存器文件）
    reg id_ex_mem_to_reg_sel;            // 数据存储器到寄存器的选择信号（控制写回数据来源）
    reg id_ex_mem_write_en;              // 数据存储器写使能（控制store类操作）
    reg id_ex_mem_read_en;               // 数据存储器读使能（控制load类操作）
    reg id_ex_alu_src_sel;               // ALU操作数选择（选择立即数或寄存器值）
    reg [1:0] id_ex_alu_op;              // ALU操作类型编码（用于ALU控制单元解码）
    reg id_ex_jal_sel;                   // JAL指令选择信号（控制链接地址写回）
    reg [2:0] id_ex_mem_op;              // 存储器操作类型（指定存取操作的字节/半字/字长度等）

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            id_ex_pc_plus_4 <= 32'b0;
            id_ex_read_data1 <= 32'b0;
            id_ex_read_data2 <= 32'b0;
            id_ex_sign_extended_imm <= 32'b0;
            id_ex_rs <=  5'b0;
            id_ex_rt <= 5'b0;
            id_ex_write_reg <= 5'b0;
            id_ex_reg_write_en <= 1'b0;
            id_ex_mem_to_reg_sel <= 1'b0;
            id_ex_mem_write_en <= 1'b0;
            id_ex_mem_read_en <= 1'b0;
            id_ex_alu_src_sel <= 1'b0;
            id_ex_alu_op <= 2'b0;
            id_ex_jal_sel <= 1'b0;
            id_ex_mem_op <= 3'b0;
        end else if (hazard_stall) begin    // 发生Load-Use冒险，插入气泡（Bubble）
            id_ex_pc_plus_4 <= 32'b0;
            id_ex_read_data1 <= 32'b0;
            id_ex_read_data2 <= 32'b0;
            id_ex_sign_extended_imm <= 32'b0;
            id_ex_rs <= 5'b0;
            id_ex_rt <= 5'b0;
            id_ex_write_reg <= 5'b0;
            // 关键：插入气泡意味着后续阶段不做任何有影响的操作
            id_ex_reg_write_en <= 1'b0;
            id_ex_mem_to_reg_sel <= 1'b0;
            id_ex_mem_write_en <= 1'b0;
            id_ex_mem_read_en <= 1'b0;      // 特别重要，清除mem_read以避免错误冒险检测
            id_ex_alu_src_sel <= 1'b0;
            id_ex_alu_op <= 2'b0;
            id_ex_jal_sel <= 1'b0;
            id_ex_mem_op <= 3'b0;
        end else begin  // 正常流动
            id_ex_pc_plus_4 <= pc_plus_4_id;
            id_ex_read_data1 <= read_data1_id;
            id_ex_read_data2 <= read_data2_id;
            id_ex_sign_extended_imm <= sign_extended_imm_id;
            id_ex_rs <= instruction_id[25:21];
            id_ex_rt <= instruction_id[20:16];
            id_ex_write_reg <= write_reg_id;
            id_ex_reg_write_en <= reg_write_en_id;
            id_ex_mem_to_reg_sel <= mem_to_reg_sel_id;
            id_ex_mem_write_en <= mem_write_en_id;
            id_ex_mem_read_en <= mem_read_en_id;
            id_ex_alu_src_sel <= alu_src_sel_id;
            id_ex_alu_op <= alu_op_id;
            id_ex_jal_sel <= jal_sel_id;
        
            if (instruction_id[31:26] == `OP_LW || instruction_id[31:26] == `OP_SW) begin
                id_ex_mem_op <= 3'b010; // 字操作编码，用于LW和SW
            end else begin
                id_ex_mem_op <= 3'b000; // 默认无操作
            end
        end
    end
    // 三、执行（EX）

    /*
    // ID/EX 流水线寄存器
    // 该模块用于在指令译码（ID）阶段和执行（EX）阶段之间暂存数据和控制信号
    reg [31:0] id_ex_pc_plus_4;          // 存储PC+4值，用于后续计算跳转地址或异常处理
    reg [31:0] id_ex_read_data1;         // 从寄存器文件读取的第一个操作数（rs值）
    reg [31:0] id_ex_read_data2;         // 从寄存器文件读取的第二个操作数（rt值）
    reg [31:0] id_ex_sign_extended_imm;  // 经符号扩展后的立即数
    reg [4:0] id_ex_rs;                  // rs寄存器编号（用于转发检测）
    reg [4:0] id_ex_rt;                  // rt寄存器编号（用于转发检测）
    reg [4:0] id_ex_write_reg;           // 目标寄存器编号（可能为rd或rt）
    reg id_ex_reg_write_en;              // 寄存器写使能信号（控制是否写回寄存器文件）
    reg id_ex_mem_to_reg_sel;            // 数据存储器到寄存器的选择信号（控制写回数据来源）
    reg id_ex_mem_write_en;              // 数据存储器写使能（控制store类操作）
    reg id_ex_mem_read_en;               // 数据存储器读使能（控制load类操作）
    reg id_ex_alu_src_sel;               // ALU操作数选择（选择立即数或寄存器值）
    reg [1:0] id_ex_alu_op;              // ALU操作类型编码（用于ALU控制单元解码）
    reg id_ex_jal_sel;                   // JAL指令选择信号（控制链接地址写回）
    reg [2:0] id_ex_mem_op;              // 存储器操作类型（指定存取操作的字节/半字/字长度等）
    */
    wire [31:0] pc_plus_4_ex = id_ex_pc_plus_4;
    wire [31:0] read_data1_ex = id_ex_read_data1;
    wire [31:0] read_data2_ex = id_ex_read_data2;
    wire [31:0] sign_extended_imm_ex = id_ex_sign_extended_imm;
    wire [4:0] rs_ex = id_ex_rs;
    wire [4:0] rt_ex = id_ex_rt;
    wire [4:0] write_reg_ex = id_ex_write_reg;
    wire reg_write_en_ex = id_ex_reg_write_en;
    wire mem_to_reg_sel_ex = id_ex_mem_to_reg_sel;
    wire mem_write_en_ex = id_ex_mem_write_en;
    wire mem_read_en_ex = id_ex_mem_read_en;
    wire alu_src_sel_ex = id_ex_alu_src_sel;
    wire [1:0] alu_op_ex = id_ex_alu_op;
    wire jal_sel_ex = id_ex_jal_sel;
    wire [2:0] mem_op_ex = id_ex_mem_op;

    // 前递单元（Forwarding Unit）
    /*
    module forwarding_unit(
    input wire [4:0] ex_rs,     // EX阶段（执行阶段）的源寄存器rs
    input wire [4:0] ex_rt,     // EX阶段（执行阶段）的源寄存器rt
    input wire [4:0] mem_rd,    // MEM阶段（内存访问阶段）的目标寄存器rd
    input wire mem_reg_write,   // MEM阶段（内存访问阶段）是否写寄存器
    input wire [4:0] wb_rd,     // WB阶段（写回阶段）的目标寄存器
    input wire wb_reg_write,    // WB阶段（写回阶段）是否写寄存器

    output reg [1:0] forward_a,    // 前递选择信号a（用于rs）
    output reg [1:0] forward_b     // 前递选择信号b（用于rt）
    );
    */
    wire [1:0] forward_a;
    wire [1:0] forward_b;

    // 来自后续阶段的信息
    wire mem_reg_write_en;
    wire [4:0] mem_rd;
    wire wb_reg_write_en;
    wire [4:0] wb_rd;

    forwarding_unit fwd_unit_instance(
        .ex_rs(rs_ex),
        .ex_rt(rt_ex),
        .mem_rd(mem_rd),
        .mem_reg_write(mem_reg_write_en),
        .wb_rd(wb_rd),
        .wb_reg_write(wb_reg_write_en),
        .forward_a(forward_a),
        .forward_b(forward_b)
    );

    // ALU 输入操作数选择（前递MUX）
    wire [31:0] alu_input_a;
    wire [31:0] alu_input_b_prev;
    /*
    module mux_3x1 #(parameter Width = 32)(
    input wire  [Width-1:0] in0,
    input wire  [Width-1:0] in1,
    input wire  [Width-1:0] in2,
    input wire  [1:0]       sel,
    output wire [Width-1:0] out
    );
    */
    mux_3x1 #(32) mux_forward_a(
        .in0(read_data1_ex),
        .in1(write_data_wb),
        .in2(alu_result_mem),
        .sel(forward_a),
        .out(alu_input_a)
    );

    mux_3x1 #(32) mux_forward_b(
        .in0(read_data2_ex),
        .in1(write_data_wb),
        .in2(alu_result_mem),
        .sel(forward_b),
        .out(alu_input_b_prev)
    );

    // ALU 源操作数选择 (立即数MUX)
    wire [31:0] alu_input_b;
    mux_2x1 #(32) mux_alu_src (
        .in0(alu_input_b_prev),
        .in1(sign_extended_imm_ex),
        .sel(alu_src_sel_ex),
        .out(alu_input_b)
    );

    // ALU 控制 (ALU Control)
    wire [3:0] alu_ctrl_signal;
    alu_control alu_ctrl_instance (
        .alu_op(alu_op_ex),
        .funct(sign_extended_imm_ex[5:0]), // 注意：对于R型，funct在立即数的低6位
        .opcode(sign_extended_imm_ex[31:26]), // I-Type opcode也在立即数的高位
        .alu_ctrl(alu_ctrl_signal)
    );

    // ALU (Arithmetic Logic Unit)
    wire [31:0] alu_result_ex;
    wire alu_zero_ex;
    alu alu_instance (
        .a(alu_input_a),
        .b(alu_input_b),
        .alu_control(alu_ctrl_signal),
        .result(alu_result_ex),
        .zero(alu_zero_ex)
    );

    // 准备传递给MEM阶段的数据
    wire [31:0] mem_write_data_ex = alu_input_b_prev; // 存储的数据使用前递后的值

    // EX/MEM 流水线寄存器
    reg [31:0] ex_mem_pc_plus_4;        // 程序计数器加4的值，用于JAL指令的返回地址计算
    reg [31:0] ex_mem_alu_result;       // ALU计算结果，可能是内存访问地址或运算结果
    reg ex_mem_alu_zero;                // ALU零标志，用于分支指令的结果判断
    reg [31:0] ex_mem_mem_write_data;   // 要写入内存的数据（来自寄存器文件的读取值）
    reg [4:0] ex_mem_write_reg;         // 目标寄存器地址（用于写回阶段）
    reg ex_mem_reg_write_en;            // 寄存器写使能信号，控制是否将结果写回寄存器文件
    reg ex_mem_mem_to_reg_sel;          // 内存到寄存器选择信号，决定写回数据来自ALU还是内存
    reg ex_mem_mem_write_en;            // 内存写使能信号，控制是否执行存储操作
    reg ex_mem_mem_read_en;             // 内存读使能信号，控制是否执行加载操作（用于冒险检测）
    reg ex_mem_jal_sel;                 // JAL指令选择信号，控制是否使用PC+4作为写回数据
    reg [2:0] ex_mem_mem_op;            // 内存操作类型编码，用于区分字节、半字、字操作

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            ex_mem_pc_plus_4 <= 32'b0;
            ex_mem_alu_result <= 32'b0;
            ex_mem_alu_zero <= 1'b0;
            ex_mem_mem_write_data <= 32'b0;
            ex_mem_write_reg <= 5'b0;
            ex_mem_reg_write_en <= 1'b0;
            ex_mem_mem_to_reg_sel <= 1'b0;
            ex_mem_mem_write_en <= 1'b0;
            ex_mem_mem_read_en <= 1'b0;
            ex_mem_jal_sel <= 1'b0;
            ex_mem_mem_op <= 3'b0;
        end else begin
            ex_mem_pc_plus_4 <= pc_plus_4_ex;
            ex_mem_alu_result <= alu_result_ex;
            ex_mem_alu_zero <= alu_zero_ex;
            ex_mem_mem_write_data <= mem_write_data_ex;
            ex_mem_write_reg <= write_reg_ex;
            ex_mem_reg_write_en <= reg_write_en_ex;
            ex_mem_mem_to_reg_sel <= mem_to_reg_sel_ex;
            ex_mem_mem_write_en <= mem_write_en_ex;
            ex_mem_mem_read_en <= mem_read_en_ex;
            ex_mem_jal_sel <= jal_sel_ex;
            ex_mem_mem_op <= mem_op_ex;
        end
    end

    assign ex_rd = ex_mem_write_reg;
    assign ex_mem_read = ex_mem_mem_read_en;

    // 四、访存（MEM）

    /*
    // EX/MEM 流水线寄存器
    reg [31:0] ex_mem_pc_plus_4;        // 程序计数器加4的值，用于JAL指令的返回地址计算
    reg [31:0] ex_mem_alu_result;       // ALU计算结果，可能是内存访问地址或运算结果
    reg ex_mem_alu_zero;                // ALU零标志，用于分支指令的结果判断
    reg [31:0] ex_mem_mem_write_data;   // 要写入内存的数据（来自寄存器文件的读取值）
    reg [4:0] ex_mem_write_reg;         // 目标寄存器地址（用于写回阶段）
    reg ex_mem_reg_write_en;            // 寄存器写使能信号，控制是否将结果写回寄存器文件
    reg ex_mem_mem_to_reg_sel;          // 内存到寄存器选择信号，决定写回数据来自ALU还是内存
    reg ex_mem_mem_write_en;            // 内存写使能信号，控制是否执行存储操作
    reg ex_mem_mem_read_en;             // 内存读使能信号，控制是否执行加载操作（用于冒险检测）
    reg ex_mem_jal_sel;                 // JAL指令选择信号，控制是否使用PC+4作为写回数据
    reg [2:0] ex_mem_mem_op;            // 内存操作类型编码，用于区分字节、半字、字操作
    */

    wire [31:0] pc_plus_4_mem = ex_mem_pc_plus_4;
    wire [31:0] alu_result_mem = ex_mem_alu_result;
    wire alu_zero_mem = ex_mem_alu_zero;
    wire [31:0] mem_write_data_mem = ex_mem_mem_write_data;
    wire [4:0] write_reg_mem = ex_mem_write_reg;
    wire reg_write_en_mem = ex_mem_reg_write_en;
    wire mem_to_reg_sel_mem = ex_mem_mem_to_reg_sel;
    wire mem_write_en_mem = ex_mem_mem_write_en;
    wire mem_read_en_mem = ex_mem_mem_read_en;
    wire jal_sel_mem = ex_mem_jal_sel;
    wire [2:0] mem_op_mem = ex_mem_mem_op;

    // 数据存储器 (Data Memory)
    wire [31:0] mem_read_data_mem;

    data_memory dmem_instance (
        .clk(clk),
        .rst_n(rst_n),
        .mem_read_en(mem_read_en_mem),
        .mem_write_en(mem_write_en_mem),
        .mem_op(mem_op_mem),
        .address(alu_result_mem),
        .write_data(mem_write_data_mem),
        .read_data(mem_read_data_mem)
    );

    // JAL 目标数据选择 (在MEM阶段准备)
    wire [31:0] jal_data_mem = (jal_sel_mem) ? pc_plus_4_mem : alu_result_mem;

    // MEM/WB 流水线寄存器
    reg [31:0] mem_wb_jal_data;
    reg [31:0] mem_wb_mem_read_data;
    reg [31:0] mem_wb_alu_result;
    reg [4:0] mem_wb_write_reg;
    reg mem_wb_reg_write_en;
    reg mem_wb_mem_to_reg_sel;
    reg mem_wb_jal_sel;

    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            mem_wb_jal_data <= 32'b0;
            mem_wb_mem_read_data <= 32'b0;
            mem_wb_alu_result <= 32'b0;;
            mem_wb_write_reg <= 5'b0;
            mem_wb_reg_write_en <= 1'b0;
            mem_wb_mem_to_reg_sel <= 1'b0;
            mem_wb_jal_sel <= 1'b0;
        end else begin
            mem_wb_jal_data <= jal_data_mem;
            mem_wb_mem_read_data <= mem_read_data_mem;
            mem_wb_alu_result <= alu_result_mem;
            mem_wb_write_reg <= write_reg_mem;
            mem_wb_reg_write_en <= reg_write_en_mem;
            mem_wb_mem_to_reg_sel <= mem_to_reg_sel_mem;
            mem_wb_jal_sel <= jal_sel_mem;
        end
    end
    
    // 输出到前递单元
    assign mem_rd = mem_wb_write_reg;
    assign mem_reg_write_en = mem_wb_reg_write_en;

    // 五、写回（WB）

    /*
    // MEM/WB 流水线寄存器
    reg [31:0] mem_wb_jal_data;
    reg [31:0] mem_wb_mem_read_data;
    reg [31:0] mem_wb_alu_result;
    reg [4:0] mem_wb_write_reg;
    reg mem_wb_reg_write_en;
    reg mem_wb_mem_to_reg_sel;
    reg mem_wb_jal_sel;
    */

    wire [31:0] jal_data_wb = mem_wb_jal_data;
    wire [31:0] mem_read_data_wb = mem_wb_mem_read_data;
    wire [31:0] alu_result_wb = mem_wb_alu_result;
    wire [4:0] write_reg_wb = mem_wb_write_reg;
    wire reg_write_en_wb_in = mem_wb_reg_write_en;
    wire mem_to_reg_sel_wb = mem_wb_mem_to_reg_sel;
    wire jal_sel_wb = mem_wb_jal_sel;

    wire [31:0] normal_write_data_wb;

    mux_2x1 #(32) mux_mem_to_reg(
        .in0(alu_result_wb),
        .in1(mem_read_data_wb),
        .sel(mem_to_reg_sel_wb),
        .out(normal_write_data_wb)
    );

    mux_2x1 #(32) mux_jal(
        .in0(normal_write_data_wb),
        .in1(jal_data_wb),
        .sel(jal_sel_wb),
        .out(write_data_wb)
    );

    assign write_addr_wb = write_reg_wb;
    assign reg_write_en_wb = reg_write_en_wb_in;

    assign wb_rd = write_addr_wb;
    assign wb_reg_write_en = reg_write_en_wb;

endmodule

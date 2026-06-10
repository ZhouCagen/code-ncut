`timescale 1ns/1ps
`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

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

    wire is_rtype;
    assign is_rtype = (opcode == `OP_RTYPE);
    
    always @(*) begin
        reg_write_en = 1'b0;
        mem_to_reg_sel = 1'b0;
        mem_write_en = 1'b0;
        mem_read_en = 1'b0;
        branch_en = 1'b0;
        alu_src_sel = 1'b0;
        reg_dst_sel = 1'b0;
        alu_op = 2'b00;
        jump_en = 1'b0;
        jal_sel = 1'b0;
        jr_sel = 1'b0;
    
        case(opcode)
            `OP_RTYPE: begin // R-Type 指令
                reg_write_en = 1'b1;
                reg_dst_sel = 1'b1;
                alu_src_sel = 1'b0;
                alu_op = 2'b10; // 表示R-type指令，需要ALU控制单元进一步解码
    
                // JR指令特殊处理
                if (funct == `FUNCT_JR) begin
                    reg_write_en = 1'b0;    // JR不写寄存器
                    jump_en = 1'b1;         // 需要跳转
                    jr_sel = 1'b1;          // 是JR指令
                end
            end
    
            // I-Type 指令
    
            // 立即数运算指令 (I-Type) 这些指令将一个寄存器中的数据与一个立即数（编码在指令中的常数）进行运算。
                // ALU 操作：这些指令同样需要 ALU 执行 ADD, AND, OR, SLT 等操作。区别在于第二个操作数来自指令中的立即数字段，需要经过符号扩展或零扩展单元处理后，再送给 ALU。
            `OP_ADDI,`OP_ADDIU: begin   //	addi $1, $2, 100	加立即数	$1 = $2 + 100 (有符号，检查溢出)        addiu $1, $2, 100	加立即数	$1 = $2 + 100 (无符号，不检查溢出)
                reg_write_en = 1'b1;
                alu_src_sel = 1'b1;
                alu_op = 2'b00; // 加法操作
            end
    
            `OP_ANDI: begin   // andi $1, $2, 0xFF	与立即数	$1 = $2 & 0xFF (0扩展立即数)
                reg_write_en = 1'b1;
                alu_src_sel = 1'b1;
                alu_op = 2'b11; // 需要特殊处理
            end
    
            `OP_ORI: begin    // ori $1, $2, 0xFF	或立即数	$1 = $2 | 0xFF
                reg_write_en = 1'b1;
                alu_src_sel = 1'b1;
                alu_op = 2'b11; // 需要特殊处理
            end
    
            `OP_XORI: begin   // xori $1, $2, 0xFF	异或立即数	$1 = $2 ^ 0xFF (0扩展立即数)
                reg_write_en = 1'b1;
                alu_src_sel = 1'b1;
                alu_op = 2'b11; // 需要特殊处理
            end
    
            `OP_SLTI: begin   // slti $1, $2, 100	小于立即数置位	if ($2 < 100) $1=1 else $1=0 (有符号比较)
                reg_write_en = 1'b1;
                alu_src_sel = 1'b1;
                alu_op = 2'b11; // 需要特殊处理
            end
    
            `OP_SLTIU: begin  // sltiu $1, $2, 100	小于立即数置位	if ($2 < 100) $1=1 else $1=0 (无符号比较
                reg_write_en = 1'b1;
                alu_src_sel = 1'b1;
                alu_op = 2'b11; // 需要特殊处理
            end
    
            `OP_LUI: begin    // lui $1, 0x1234	加载高位立即数	$1 = 0x1234 << 16 (将16位立即数放到高16位，低16位为0)
                reg_write_en = 1'b1;
                alu_src_sel = 1'b1;
                alu_op = 2'b11; // 需要特殊处理
            end
    
            // 数据传输指令 (I-Type) - 回答你的核心问题 
                // ALU 在这两条指令中的作用：
                //     地址计算：ALU 的核心任务是执行 $2 + 100。这里的 100 是指令中的16位偏移量，需要先进行符号扩展成32位，再和 $2 的值一起送入 ALU 执行 ADD 操作。
                //     结果：ALU 输出的32位结果就是访问内存的有效地址。
                //     内存访问：对于 lw，控制单元会使用这个地址从数据内存中读取数据，然后通过另一个多路选择器写回到寄存器 $1。
                //              对于 sw，控制单元会使用这个地址，将寄存器 $1 的值写入数据内存。
                // 所以，lw/sw 本身不是 ALU 操作，但它们依赖 ALU 来完成最关键的地址计算工作。
            `OP_LW: begin     // lw $1, 100($2)	  从内存加载字	 $1 = Memory[$2 + 100]
                reg_write_en = 1'b1;
                mem_to_reg_sel = 1'b1;
                alu_src_sel = 1'b1;
                mem_read_en = 1'b1;
                alu_op = 2'b00; // 加法操作
            end
    
            `OP_SW: begin     // sw $1, 100($2)	  向内存存储字	 Memory[$2 + 100] = $1
                alu_src_sel = 1'b1;
                mem_write_en = 1'b1;
                alu_op = 2'b00; // 加法操作
            end
    
            // 分支指令 (I-Type) 根据条件决定是否改变程序执行流程（跳转）
                // ALU 的作用：ALU 需要执行 SUB 操作来计算 $1 - $2，并产生 zero 标志位。控制单元根据 zero 标志位的结果来决定是否跳转。同时，跳转地址的计算 PC + 4 + (offset << 2) 也可能需要另一个加法器（或复用 ALU）来完成。
            `OP_BEQ: begin  // beq $1, $2, label    相等则分支     if ($1 == $2) jump to label
                branch_en = 1'b1;
                alu_op = 2'b01; // 减法操作
            end
    
            `OP_BNE: begin  // bne $1, $2, label    不等则分支     if ($1 != $2) jump to label
                branch_en = 1'b1;
                alu_op = 2'b01; // 减法操作
            end
    
            // J-Type 指令
                // ALU 的作用：ALU 需要执行 SUB 操作来计算 $1 - $2，并产生 zero 标志位。控制单元根据 zero 标志位的结果来决定是否跳转。同时，跳转地址的计算 PC + 4 + (offset << 2) 也可能需要另一个加法器（或复用 ALU）来完成。
            //跳转指令 (J-Type) 无条件改变程序执行流程。
            `OP_J: begin    // j label	跳转	直接跳转到 label 处执行
                jump_en = 1'b1;
            end
    
            `OP_JAL: begin  // jal func	跳转并链接	跳转到 func，同时将返回地址(PC+4)存入 $ra($31)
                jump_en = 1'b1;
                jal_sel = 1'b1;
                reg_write_en = 1'b1;
            end
    
            default: begin  
                reg_write_en = 1'b0;
                mem_to_reg_sel = 1'b0;
                mem_write_en = 1'b0;
                mem_read_en = 1'b0;
                branch_en = 1'b0;
                alu_src_sel = 1'b0;
                reg_dst_sel = 1'b0;
                alu_op = 2'b00;
                jump_en = 1'b0;
                jal_sel = 1'b0;
                jr_sel = 1'b0;
            end
        endcase
    end        
endmodule
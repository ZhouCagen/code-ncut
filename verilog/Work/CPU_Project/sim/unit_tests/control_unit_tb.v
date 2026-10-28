`timescale 1ns/1ps
`include "E:/Vivado/Work/CPU_Project/src/defines.vh"

module control_unit_tb;

    // 输入
    reg [5:0] opcode;
    reg [5:0] funct;
    
    // 输出
    wire reg_write_en;
    wire mem_to_reg_sel;
    wire mem_write_en;
    wire mem_read_en;
    wire branch_en;
    wire alu_src_sel;
    wire reg_dst_sel;
    wire [1:0] alu_op;
    wire jump_en;
    wire jal_sel;
    wire jr_sel;
    
    // 实例化被测试单元
    control_unit uut (
        .opcode(opcode),
        .funct(funct),
        .reg_write_en(reg_write_en),
        .mem_to_reg_sel(mem_to_reg_sel),
        .mem_write_en(mem_write_en),
        .mem_read_en(mem_read_en),
        .branch_en(branch_en),
        .alu_src_sel(alu_src_sel),
        .reg_dst_sel(reg_dst_sel),
        .alu_op(alu_op),
        .jump_en(jump_en),
        .jal_sel(jal_sel),
        .jr_sel(jr_sel)
    );
    
    // 测试过程
    initial begin
        // 打开VCD文件用于波形查看
        $dumpfile("control_unit_tb.vcd");
        $dumpvars(0, control_unit_tb);
        
        // 初始化输入
        opcode = 0;
        funct = 0;
        
        // 监视输出 - 使用英文标签避免乱码
        $monitor("Time=%0t op=%h fn=%h | rwe=%b m2r=%b mwe=%b mre=%b br=%b as=%b rd=%b aop=%b j=%b js=%b jrs=%b", 
                 $time, opcode, funct, reg_write_en, mem_to_reg_sel, mem_write_en, mem_read_en, 
                 branch_en, alu_src_sel, reg_dst_sel, alu_op, jump_en, jal_sel, jr_sel);
        
        // 测试R型指令-ADD
        #10;
        opcode = `OP_RTYPE;
        funct = `FUNCT_ADD;
        
        // 测试R型指令-SUB
        #10;
        funct = `FUNCT_SUB;
        
        // 测试R型指令-AND
        #10;
        funct = `FUNCT_AND;
        
        // 测试R型指令-JR
        #10;
        funct = `FUNCT_JR;
        
        // 测试LW指令
        #10;
        opcode = `OP_LW;
        funct = 0;
        
        // 测试SW指令
        #10;
        opcode = `OP_SW;
        
        // 测试ADDI指令
        #10;
        opcode = `OP_ADDI;
        
        // 测试ANDI指令
        #10;
        opcode = `OP_ANDI;
        
        // 测试BEQ指令
        #10;
        opcode = `OP_BEQ;
        
        // 测试J指令
        #10;
        opcode = `OP_J;
        
        // 测试JAL指令
        #10;
        opcode = `OP_JAL;
        
        // 测试LUI指令
        #10;
        opcode = `OP_LUI;
        
        // 结束测试
        #10;
        $finish;
    end

endmodule
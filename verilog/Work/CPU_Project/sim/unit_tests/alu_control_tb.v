`timescale 1ns/1ps
`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module alu_control_tb;

    // 输入
    reg [1:0] alu_op;
    reg [5:0] funct;
    reg [5:0] opcode;
    
    // 输出
    wire [3:0] alu_ctrl;
    
    // 实例化被测试单元
    alu_control uut (
        .alu_op(alu_op),
        .funct(funct),
        .opcode(opcode),
        .alu_ctrl(alu_ctrl)
    );
    
    // 测试过程
    initial begin
        // 打开VCD文件用于波形查看
        $dumpfile("alu_control_tb.vcd");
        $dumpvars(0, alu_control_tb);
        
        // 初始化输入
        alu_op = 0;
        funct = 0;
        opcode = 0;
        
        // 监视输出 - 使用英文标签避免乱码
        $monitor("Time=%0t aop=%b fn=%h op=%h | actrl=%h", 
                 $time, alu_op, funct, opcode, alu_ctrl);
        
        // 测试加法操作 (alu_op=00)
        #10;
        alu_op = 2'b00;
        
        // 测试减法操作 (alu_op=01)
        #10;
        alu_op = 2'b01;
        
        // 测试R-type指令 (alu_op=10)
        #10;
        alu_op = 2'b10;
        
        // 测试ADD指令
        #10;
        funct = `FUNCT_ADD;
        
        // 测试SUB指令
        #10;
        funct = `FUNCT_SUB;
        
        // 测试AND指令
        #10;
        funct = `FUNCT_AND;
        
        // 测试OR指令
        #10;
        funct = `FUNCT_OR;
        
        // 测试SLT指令
        #10;
        funct = `FUNCT_SLT;
        
        // 测试SLL指令
        #10;
        funct = `FUNCT_SLL;
        
        // 测试特殊I-type指令 (alu_op=11)
        #10;
        alu_op = 2'b11;
        
        // 测试ANDI指令
        #10;
        opcode = `OP_ANDI;
        
        // 测试ORI指令
        #10;
        opcode = `OP_ORI;
        
        // 测试SLTI指令
        #10;
        opcode = `OP_SLTI;
        
        // 测试LUI指令
        #10;
        opcode = `OP_LUI;
        
        // 测试无效操作码
        #10;
        opcode = 6'b111111; // 无效操作码
        
        // 结束测试
        #10;
        $finish;
    end

endmodule
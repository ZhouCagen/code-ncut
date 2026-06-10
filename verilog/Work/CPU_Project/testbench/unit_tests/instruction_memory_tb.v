`timescale 1ns / 1ps
    
`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module instruction_memory_tb;

    // 测试信号
    reg [31:0] addr;
    wire [31:0] instruction;
    
    // 实例化被测试模块
    instruction_memory uut (
        .addr(addr),
        .instruction(instruction)
    );
    
    // 测试过程
    initial begin
        // 记录波形
        $dumpfile("instruction_memory_tb.vcd");
        $dumpvars(0, instruction_memory_tb);
        
        // 测试1: 读取前几条指令
        $display("Test 1: Read first few instructions");
        addr = 32'h00000000;
        #10;
        $display("Address %h: Instruction = %h", addr, instruction);
        
        addr = 32'h00000004;
        #10;
        $display("Address %h: Instruction = %h", addr, instruction);
        
        addr = 32'h00000008;
        #10;
        $display("Address %h: Instruction = %h", addr, instruction);
        
        // 测试2: 读取随机地址
        $display("Test 2: Read random addresses");
        addr = 32'h00000010;
        #10;
        $display("Address %h: Instruction = %h", addr, instruction);
        
        addr = 32'h00000020;
        #10;
        $display("Address %h: Instruction = %h", addr, instruction);
        
        // 测试3: 边界测试
        $display("Test 3: Boundary test");
        addr = 32'h00000FFC; // 最后一个字地址
        #10;
        $display("Address %h: Instruction = %h", addr, instruction);
        
        $display("All tests completed");
        $finish;
    end

endmodule
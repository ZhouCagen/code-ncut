`timescale 1ns / 1ps
    
`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module program_counter_tb;

    // 测试信号
    reg clk;
    reg rst;
    reg stall;
    reg branch_taken;
    reg jump;
    reg jump_reg;
    reg [31:0] branch_addr;
    reg [31:0] jump_addr;
    reg [31:0] jump_reg_addr;
    
    wire [31:0] pc;
    
    // 实例化被测试模块
    program_counter uut (
        .clk(clk),
        .rst(rst),
        .stall(stall),
        .branch_taken(branch_taken),
        .jump(jump),
        .jump_reg(jump_reg),
        .branch_addr(branch_addr),
        .jump_addr(jump_addr),
        .jump_reg_addr(jump_reg_addr),
        .pc(pc)
    );
    
    // 时钟生成
    always #5 clk = ~clk;
    
    // 测试过程
    initial begin
        // 初始化信号
        clk = 0;
        rst = 0;
        stall = 0;
        branch_taken = 0;
        jump = 0;
        jump_reg = 0;
        branch_addr = 0;
        jump_addr = 0;
        jump_reg_addr = 0;
        
        // 记录波形
        $dumpfile("program_counter_tb.vcd");
        $dumpvars(0, program_counter_tb);
        
        // 测试1: 复位测试
        $display("Test 1: Reset");
        rst = 1;
        #12; // 稍微超过一个时钟周期
        $display("PC = %h", pc);
        rst = 0;
        #10;
        
        // 测试2: 顺序执行
        $display("Test 2: Sequential execution");
        // 等待一个时钟周期
        #10;
        $display("After 1st clock: PC = %h", pc);
        // 等待另一个时钟周期
        #10;
        $display("After 2nd clock: PC = %h", pc);
        
        // 测试3: 分支跳转
        $display("Test 3: Branch");
        branch_taken = 1;
        branch_addr = 32'h0000_1000;
        #10;
        $display("PC = %h", pc);
        branch_taken = 0;
        #10;
        
        // 测试4: 跳转
        $display("Test 4: Jump");
        jump = 1;
        jump_addr = 32'h0000_2000;
        #10;
        $display("PC = %h", pc);
        jump = 0;
        #10;
        
        // 测试5: 暂停
        $display("Test 5: Stall");
        // 记录当前PC值
        $display("Before stall: PC = %h", pc);
        stall = 1;
        #20;
        $display("During stall: PC = %h", pc);
        stall = 0;
        #10;
        
        $finish;
    end

endmodule
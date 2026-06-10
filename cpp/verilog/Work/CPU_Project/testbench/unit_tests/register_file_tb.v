`timescale 1ns/1ps

module register_file_tb;

    // 测试信号
    reg clk;
    reg rst_n;
    reg write_enable;
    reg [4:0] read_addr1;
    reg [4:0] read_addr2;
    reg [4:0] write_addr;
    reg [31:0] write_data;
    wire [31:0] read_data1;
    wire [31:0] read_data2;
    
    // 实例化被测模块
    register_file uut (
        .clk(clk),
        .rst_n(rst_n),
        .write_enable(write_enable),
        .read_addr1(read_addr1),
        .read_addr2(read_addr2),
        .write_addr(write_addr),
        .write_data(write_data),
        .read_data1(read_data1),
        .read_data2(read_data2)
    );
    
    // 时钟生成
    always #5 clk = ~clk;
    
    // 测试过程
    initial begin
        // 初始化信号
        clk = 0;
        rst_n = 0;
        write_enable = 0;
        read_addr1 = 0;
        read_addr2 = 0;
        write_addr = 0;
        write_data = 0;
        
        // 波形记录
        $dumpfile("register_file_tb.vcd");
        $dumpvars(0, register_file_tb);
        
        // 测试1: 复位测试
        #10;
        rst_n = 1;
        #10;
        
        // 测试2: 读取$zero寄存器
        read_addr1 = 5'b0;
        #10;
        if (read_data1 !== 32'b0) 
            $display("Error: $zero register read value is not zero");
        else
            $display("Test passed: $zero register read value is zero");
        
        // 测试3: 写入和读取寄存器
        write_enable = 1;
        write_addr = 5'b1;     // 写入$at寄存器
        write_data = 32'h12345678;
        #10;
        
        read_addr1 = 5'b1;     // 读取$at寄存器
        #5;
        if (read_data1 !== 32'h12345678)
            $display("Error: Register write and read values do not match");
        else
            $display("Test passed: Register write and read values match");
        
        // 测试4: 同时读取两个寄存器
        write_addr = 5'b10;    // 写入$v0寄存器
        write_data = 32'habcdef12;
        #10;
        
        read_addr1 = 5'b1;     // 读取$at寄存器
        read_addr2 = 5'b10;    // 读取$v0寄存器
        #5;
        if (read_data1 !== 32'h12345678 || read_data2 !== 32'habcdef12)
            $display("Error: Dual-port read values do not match");
        else
            $display("Test passed: Dual-port read values match");
        
        // 测试5: 尝试写入$zero寄存器
        write_addr = 5'b0;     // 尝试写入$zero寄存器
        write_data = 32'hffffffff;
        #10;
        
        read_addr1 = 5'b0;     // 读取$zero寄存器
        #5;
        if (read_data1 !== 32'b0)
            $display("Error: $zero register was written to");
        else
            $display("Test passed: $zero register remains zero");
        
        // 测试6: 写使能无效时不应写入
        write_enable = 0;
        write_addr = 5'b11;    // 尝试写入$v1寄存器
        write_data = 32'hdeadbeef;
        #10;
        
        read_addr1 = 5'b11;    // 读取$v1寄存器
        #5;
        if (read_data1 !== 32'b0)
            $display("Error: Register written when write enable is disabled");
        else
            $display("Test passed: Register not written when write enable is disabled");
        
        // 测试7: 异步读取测试
        write_enable = 1;
        write_addr = 5'b100;   // 写入$a0寄存器
        write_data = 32'h11111111;
        #5; // 等待半个时钟周期
        
        // 在时钟上升沿之前检查读取是否立即生效
        read_addr1 = 5'b100;
        #1;
        if (read_data1 !== 32'h11111111)
            $display("Error: Asynchronous read did not take effect immediately");
        else
            $display("Test passed: Asynchronous read took effect immediately");
        
        #4; // 完成时钟周期
        
        // 完成测试
        #10;
        $display("All tests complete");
        $finish;
    end

endmodule

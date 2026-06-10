/*
`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module data_memory_tb;

    // 输入信号
    reg clk;
    reg rst_n;
    reg mem_read_en;
    reg mem_write_en;
    reg [2:0] mem_op;           // 使用3位操作码
    reg [31:0] address;
    reg [31:0] write_data;
    
    // 输出信号
    wire [31:0] read_data;
    
    // 实例化被测模块
    data_memory uut (
        .clk(clk),
        .rst_n(rst_n),
        .mem_read_en(mem_read_en),
        .mem_write_en(mem_write_en),
        .mem_op(mem_op),
        .address(address),
        .write_data(write_data),
        .read_data(read_data)
    );
    
    // 时钟生成
    always #5 clk = ~clk;
    
    // 测试序列
    initial begin
        // 初始化信号
        clk = 0;
        rst_n = 0;
        mem_read_en = 0;
        mem_write_en = 0;
        mem_op = 0;
        address = 0;
        write_data = 0;
        
        // 记录波形
        $dumpfile("data_memory_tb.vcd");
        $dumpvars(0, data_memory_tb);
        
        // 释放复位
        #10 rst_n = 1;
        
        // 测试1: 无符号字节写入和读取
        #10;
        mem_write_en = 1;
        mem_op = `MEM_OP_LBYTE;  // 使用LBYTE进行写入（写入操作使用低2位判断宽度）
        address = 32'h10;
        write_data = 32'h000000A5;
        #10;
        mem_write_en = 0;
        mem_read_en = 1;
        mem_op = `MEM_OP_LBYTEU; // 使用LBYTEU进行无符号读取
        #10;
        $display("Test 1 - Unsigned Byte Write/Read: Address=%h, Written=%h, Read=%h", 
                 address, write_data, read_data);
        
        // 测试2: 无符号半字写入和读取
        #10;
        mem_read_en = 0;
        mem_write_en = 1;
        mem_op = `MEM_OP_LHALF;  // 使用LHALF进行写入
        address = 32'h20;
        write_data = 32'h0000BEEF;
        #10;
        mem_write_en = 0;
        mem_read_en = 1;
        mem_op = `MEM_OP_LHALFU; // 使用LHALFU进行无符号读取
        #10;
        $display("Test 2 - Unsigned Halfword Write/Read: Address=%h, Written=%h, Read=%h", 
                 address, write_data, read_data);
        
        // 测试3: 字写入和读取
        #10;
        mem_read_en = 0;
        mem_write_en = 1;
        mem_op = `MEM_OP_LWORD;  // 使用LWORD进行写入
        address = 32'h30;
        write_data = 32'hDEADBEEF;
        #10;
        mem_write_en = 0;
        mem_read_en = 1;
        mem_op = `MEM_OP_LWORD;  // 使用LWORD进行读取
        #10;
        $display("Test 3 - Word Write/Read: Address=%h, Written=%h, Read=%h", 
                 address, write_data, read_data);
        
        // 测试4: 有符号字节（测试符号扩展）
        #10;
        mem_read_en = 0;
        mem_write_en = 1;
        mem_op = `MEM_OP_LBYTE;  // 使用LBYTE进行写入
        address = 32'h40;
        write_data = 32'hFFFFFF87; // -121的补码表示
        #10;
        mem_write_en = 0;
        mem_read_en = 1;
        mem_op = `MEM_OP_LBYTE;  // 使用LBYTE进行有符号读取
        #10;
        $display("Test 4 - Signed Byte: Address=%h, Written=%h, Read=%h", 
                 address, write_data, read_data);
        
        // 测试5: 有符号半字（测试符号扩展）
        #10;
        mem_read_en = 0;
        mem_write_en = 1;
        mem_op = `MEM_OP_LHALF;  // 使用LHALF进行写入
        address = 32'h50;
        write_data = 32'hFFFF8000; // -32768的补码表示
        #10;
        mem_write_en = 0;
        mem_read_en = 1;
        mem_op = `MEM_OP_LHALF;  // 使用LHALF进行有符号读取
        #10;
        $display("Test 5 - Signed Halfword: Address=%h, Written=%h, Read=%h", 
                 address, write_data, read_data);
        
        // 结束测试
        #10;
        $display("Data Memory Test Completed");
        $finish;
    end

endmodule
*/

`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module data_memory_tb;

    // 输入信号
    reg clk;
    reg rst_n;
    reg mem_read_en;
    reg mem_write_en;
    reg [2:0] mem_op;           // 使用3位操作码
    reg [31:0] address;
    reg [31:0] write_data;
    
    // 输出信号
    wire [31:0] read_data;
    
    // 实例化被测模块
    data_memory uut (
        .clk(clk),
        .rst_n(rst_n),
        .mem_read_en(mem_read_en),
        .mem_write_en(mem_write_en),
        .mem_op(mem_op),
        .address(address),
        .write_data(write_data),
        .read_data(read_data)
    );
    
    // 时钟生成
    always #5 clk = ~clk;
    
    // 测试序列
    initial begin
        // 初始化信号
        clk = 0;
        rst_n = 0;
        mem_read_en = 0;
        mem_write_en = 0;
        mem_op = 0;
        address = 0;
        write_data = 0;
        
        // 记录波形
        $dumpfile("data_memory_tb.vcd");
        $dumpvars(0, data_memory_tb);
        
        // 释放复位
        #10 rst_n = 1;
        
        // 测试1: 字写入和读取 (LW/SW指令)
        #10;
        mem_write_en = 1;
        mem_op = 3'b010;  // 字操作编码
        address = 32'h10;
        write_data = 32'hDEADBEEF;
        #10;
        mem_write_en = 0;
        mem_read_en = 1;
        #10;
        $display("Test 1 - Word Write/Read: Address=%h, Written=%h, Read=%h", 
                 address, write_data, read_data);
        
        // 测试2: 另一个字的写入和读取
        #10;
        mem_read_en = 0;
        mem_write_en = 1;
        address = 32'h20;
        write_data = 32'hCAFEBABE;
        #10;
        mem_write_en = 0;
        mem_read_en = 1;
        #10;
        $display("Test 2 - Word Write/Read: Address=%h, Written=%h, Read=%h", 
                 address, write_data, read_data);
        
        // 测试3: 读取未写入的地址
        #10;
        mem_read_en = 1;
        address = 32'h30; // 未初始化的地址
        #10;
        $display("Test 3 - Read Uninitialized: Address=%h, Read=%h", 
                 address, read_data);
        
        // 测试4: 复位后读取
        #10;
        rst_n = 0;
        #10;
        rst_n = 1;
        mem_read_en = 1;
        address = 32'h10; // 之前写入的地址
        #10;
        $display("Test 4 - Read After Reset: Address=%h, Read=%h", 
                 address, read_data);
        
        // 结束测试
        #10;
        $display("Data Memory Test Completed");
        $finish;
    end

endmodule
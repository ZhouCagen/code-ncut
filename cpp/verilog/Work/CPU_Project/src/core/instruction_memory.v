`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module instruction_memory(
    input wire [31:0] addr,         // 指令地址输入
    output reg [31:0] instruction
);

    reg [31:0] mem[0:1023];

    initial begin
        $readmemh("E:/Vivado/Work/CPU_Project/testbench/imem_init.txt", mem);
    end

    // 异步读取
    always @(*) begin
        instruction = mem[addr[11:2]];
    end

endmodule    
`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module program_counter(
    input wire clk,
    input wire rst_n,
    input wire stall,                   // 流水线暂停信号
    input wire branch_taken,            // 分支跳转信号
    input wire jump,                    // 跳转信号
    input wire jump_reg,                // 寄存器跳转信号
    input wire [31:0] branch_addr,
    input wire [31:0] jump_addr,
    input wire [31:0] jump_reg_addr,

    output reg [31:0] pc                // 程序计数器输出
);

    // 程序计数器更新逻辑
    always @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            pc <= `PC_INIT;
        end else if (stall) begin
            pc <= pc;
        end else if (jump_reg) begin
            pc <= jump_reg_addr;
        end else if (jump) begin
            pc <= jump_addr;
        end else if (branch_taken) begin
            pc <= branch_addr;
        end else begin
            pc <= pc + 4;
        end
    end

endmodule
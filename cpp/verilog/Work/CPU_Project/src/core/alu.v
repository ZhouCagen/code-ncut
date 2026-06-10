`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module alu (
    input [31:0] a,
    input [31:0] b,
    input [3:0] alu_control,
    output reg [31:0] result,
    output zero
);
    
    always @(*) begin
        case(alu_control)
            `ALU_ADD : result = a + b;
            `ALU_SUB : result = a - b;
            `ALU_AND : result = a & b;
            `ALU_OR  : result = a | b;
            `ALU_XOR : result = a ^ b;
            `ALU_NOR : result = ~(a | b);
            `ALU_SLT : result = ($signed(a) < $signed(b)) ? 32'd1 : 32'd0;
            `ALU_SLTU: result = (a < b) ? 32'd1 : 32'd0;
            `ALU_SLL : result = b << a[4:0];
            `ALU_SRL : result = b >> a[4:0];
            `ALU_SRA : result = $signed(b) >>> a[4:0];
            `ALU_LUI : result = {b[15:0], 16'b0};
            default  : result = 32'bx;
        endcase
    end

    assign zero = (result == 32'b0);
endmodule
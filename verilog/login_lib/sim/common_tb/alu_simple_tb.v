`timescale 1ns/1ps

module alu_simple_tb;

    reg [3:0] A, B;
    reg [2:0] ALU_Sel;
    wire [3:0] Result;
    wire       Cout;  
    
    alu_simple uut(
        .A(A), .B(B),
        .ALU_Sel(ALU_Sel),
        .Result(Result),
        .Cout(Cout)
    );
    
    initial begin 
        $display("=== 简单 ALU 测试 ===");
        $display("A  B  Sel | Result Cout");
        $display("----------------------");

        A = 4'd7; B = 4'd5; ALU_Sel = 3'b000; #10; $display("%d  %d  %b |   %d   %b", A,B,ALU_Sel,Result,Cout);
        ALU_Sel = 3'b001; #10; $display("%d  %d  %b |   %d   %b", A,B,ALU_Sel,Result,Cout);
        ALU_Sel = 3'b010; #10; $display("%d  %d  %b |   %d   %b", A,B,ALU_Sel,Result,Cout);
        ALU_Sel = 3'b011; #10; $display("%d  %d  %b |   %d   %b", A,B,ALU_Sel,Result,Cout);
        ALU_Sel = 3'b100; #10; $display("%d  %d  %b |   %d   %b", A,B,ALU_Sel,Result,Cout);
        ALU_Sel = 3'b101; #10; $display("%d  %d  %b |   %d   %b", A,B,ALU_Sel,Result,Cout);

        $display("=== 测试结束 ===");
        $finish;
    end

endmodule 
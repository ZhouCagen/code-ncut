`timescale 1ns/1ps

module mux4_tb;

    reg I0, I1, I2, I3;
    reg [1:0] S;
    wire Y;

    mux4 uut(
        .I0(I0), .I1(I1), .I2(I2), .I3(I3),
        .S(S),
        .Y(Y)
    );
    
    initial begin
        $display("=== 4:1 MUX 测试 ===");
        $display("S1 S0 | Y");
        $display("-----------");

        I0=0; I1=1; I2=0; I3=1;

        S = 2'b00; #10; $display("%b %b | %b", S[1], S[0], Y);
        S = 2'b01; #10; $display("%b %b | %b", S[1], S[0], Y);
        S = 2'b10; #10; $display("%b %b | %b", S[1], S[0], Y);
        S = 2'b11; #10; $display("%b %b | %b", S[1], S[0], Y);

        $display("=== 测试结束 ===");
        $finish;
    end
endmodule
`timescale 1ns/1ps

module adder4_dec_tb;

reg [3:0] A3, A2, A1, A0;
reg [3:0] B3, B2, B1, B0;
reg       Cin;
wire [3:0] Sum3, Sum2, Sum1, Sum0;
wire       Cout;

adder4_dec uut(
    .A3(A3), .A2(A2), .A1(A1), .A0(A0),
    .B3(B3), .B2(B2), .B1(B1), .B0(B0),
    .Cin(Cin),
    .Sum3(Sum3), .Sum2(Sum2), .Sum1(Sum1), .Sum0(Sum0),
    .Cout(Cout)
);

initial begin
    $display("=== 4 位 BCD 加法器测试 ===");
    $display(" 时间(ns) |  A   |  B   | Cin |  Sum  | Cout ");
    $display("-----------------------------------------------");

    A3=0; A2=0; A1=0; A0=7;
    B3=0; B2=0; B1=0; B0=5;
    Cin=0; #10;
    $display("%8t | %d%d%d%d | %d%d%d%d |  %b  | %d%d%d%d |  %b",
             $time, A3,A2,A1,A0, B3,B2,B1,B0, Cin, Sum3,Sum2,Sum1,Sum0, Cout);

    A3=0; A2=0; A1=3; A0=4;
    B3=0; B2=0; B1=4; B0=7;
    Cin=1; #10;
    $display("%8t | %d%d%d%d | %d%d%d%d |  %b  | %d%d%d%d |  %b",
             $time, A3,A2,A1,A0, B3,B2,B1,B0, Cin, Sum3,Sum2,Sum1,Sum0, Cout);

    $display("=== 测试结束 ===");
    $finish;
    end
    
endmodule
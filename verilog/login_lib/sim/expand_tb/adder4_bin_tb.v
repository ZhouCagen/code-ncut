`timescale 1ns/1ps

module adder4_bin_tb;

reg  [3:0] A, B;
reg        Cin;
wire [3:0] Sum;
wire       Cout;

adder4_bin uut(
    .A(A),
    .B(B),
    .Cin(Cin),
    .Sum(Sum),
    .Cout(Cout)
);

initial begin
    $display("=== 4位二进制加法器测试开始 ===");
    $display("Time |   A    B  Cin |  Sum  Cout");
    $display("---------------------------------");

    $monitor("%4t | %b %b   %b  | %b    %b", 
              $time, A, B, Cin, Sum, Cout);

    A=4'b0001; B=4'b0010; Cin=0; #10;  // 1 + 2
    A=4'b0111; B=4'b0001; Cin=0; #10;  // 7 + 1
    A=4'b1111; B=4'b0001; Cin=0; #10;  // 15 + 1
    A=4'b1010; B=4'b0101; Cin=1; #10;  // 10 + 5 + 1
    A=4'b1111; B=4'b1111; Cin=1; #10;  // 15 + 15 + 1

    #10 $display("=== 测试结束 ===");
    #10 
    $finish;
    
end

endmodule
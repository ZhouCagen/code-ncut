`timescale 1ns / 1ps

module comparator_1bit_tb;

    reg A, B;
    wire A_gt_B, A_eq_B, A_lt_B;
    
    comparator_1bit uut(
        .A(A),
        .B(B),
        .Cin_gt(0),   
        .Cin_eq(1),
        .Cin_lt(0),
        .A_gt_B(A_gt_B),
        .A_eq_B(A_eq_B),
        .A_lt_B(A_lt_B)
    );
    
initial begin

        $display("=== 4 位数值比较器测试（1 位示例） ===");
        $display("Time(ns) | A B | A>B A==B A<B");
        $display("-------------------------------");

        A=0; B=0; #10;
        $display("%8t | %b %b |   %b    %b    %b", $time, A, B, A_gt_B, A_eq_B, A_lt_B);

        A=0; B=1; #10;
        $display("%8t | %b %b |   %b    %b    %b", $time, A, B, A_gt_B, A_eq_B, A_lt_B);

        A=1; B=0; #10;
        $display("%8t | %b %b |   %b    %b    %b", $time, A, B, A_gt_B, A_eq_B, A_lt_B);

        A=1; B=1; #10;
        $display("%8t | %b %b |   %b    %b    %b", $time, A, B, A_gt_B, A_eq_B, A_lt_B);

        $display("=== 测试结束 ===");
        $stop;
    end
endmodule
    
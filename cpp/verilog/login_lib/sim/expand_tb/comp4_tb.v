`timescale 1ns/1ps

module comp4_tb;
    reg [3:0] A, B;
    wire A_gt_B, A_eq_B, A_lt_B;
    comp4 uut(
        .A(A),
        .B(B),
        .A_gt_B(A_gt_B),
        .A_eq_B(A_eq_B),
        .A_lt_B(A_lt_B)
    );

    initial begin
        $display("=== 4 位数值比较器测试 ===");
        $display(" A    B   | A>B A= B<");
        $display("---------------------");

        A=4'd5; B=4'd3; #10; $display("%4d %4d |  %b    %b    %b", A,B,A_gt_B,A_eq_B,A_lt_B);
        A=4'd2; B=4'd2; #10; $display("%4d %4d |  %b    %b    %b", A,B,A_gt_B,A_eq_B,A_lt_B);
        A=4'd1; B=4'd6; #10; $display("%4d %4d |  %b    %b    %b", A,B,A_gt_B,A_eq_B,A_lt_B);
        A=4'd9; B=4'd9; #10; $display("%4d %4d |  %b    %b    %b", A,B,A_gt_B,A_eq_B,A_lt_B);

        $display("=== 测试结束 ===");
        $finish;
    end 
    
endmodule    
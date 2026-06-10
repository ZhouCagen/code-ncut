`timescale 1ns/1ps

module cmpN_bin_tb;
    parameter N = 8;
    
    reg  [N-1:0] A;
    reg  [N-1:0] B;
    wire A_gt_B;
    wire A_eq_B;
    wire A_lt_B;

    // 实例化被测设计
    cmpN_bin #(.N(N)) uut (
        .A(A),
        .B(B),
        .A_gt_B(A_gt_B),
        .A_eq_B(A_eq_B),
        .A_lt_B(A_lt_B)
    );

    // 计算期望结果
    function [2:0] calculate_expected;
        input [N-1:0] a;
        input [N-1:0] b;
        begin
            calculate_expected[2] = (a > b);  // A_gt_B
            calculate_expected[1] = (a == b); // A_eq_B
            calculate_expected[0] = (a < b);  // A_lt_B
        end
    endfunction

    integer i;
    reg [2:0] expected;
    reg is_correct;
    
    initial begin
        $display("Time(ns)\tA\t\tB\t\tExp_gt\tExp_eq\tExp_lt\tAct_gt\tAct_eq\tAct_lt\tCorrect");
        $display("====================================================================================");
        
        // 特定测试用例
        A = 8'b00000000; B = 8'b00000000; #10;
        expected = calculate_expected(A, B);
        is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
        $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                 $time, A, B, 
                 expected[2], expected[1], expected[0],
                 A_gt_B, A_eq_B, A_lt_B,
                 is_correct ? "right" : "wrong");
        
        A = 8'b00000001; B = 8'b00000000; #10;
        expected = calculate_expected(A, B);
        is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
        $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                 $time, A, B, 
                 expected[2], expected[1], expected[0],
                 A_gt_B, A_eq_B, A_lt_B,
                 is_correct ? "right" : "wrong");
        
        A = 8'b00000000; B = 8'b00000001; #10;
        expected = calculate_expected(A, B);
        is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
        $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                 $time, A, B, 
                 expected[2], expected[1], expected[0],
                 A_gt_B, A_eq_B, A_lt_B,
                 is_correct ? "right" : "wrong");
        
        A = 8'b11111111; B = 8'b00000000; #10;
        expected = calculate_expected(A, B);
        is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
        $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                 $time, A, B, 
                 expected[2], expected[1], expected[0],
                 A_gt_B, A_eq_B, A_lt_B,
                 is_correct ? "right" : "wrong");
        
        A = 8'b10101010; B = 8'b01010101; #10;
        expected = calculate_expected(A, B);
        is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
        $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                 $time, A, B, 
                 expected[2], expected[1], expected[0],
                 A_gt_B, A_eq_B, A_lt_B,
                 is_correct ? "right" : "wrong");
        
        A = 8'b01010101; B = 8'b10101010; #10;
        expected = calculate_expected(A, B);
        is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
        $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                 $time, A, B, 
                 expected[2], expected[1], expected[0],
                 A_gt_B, A_eq_B, A_lt_B,
                 is_correct ? "right" : "wrong");
        
        // 边界测试
        A = 8'b10000000; B = 8'b01111111; #10;
        expected = calculate_expected(A, B);
        is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
        $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                 $time, A, B, 
                 expected[2], expected[1], expected[0],
                 A_gt_B, A_eq_B, A_lt_B,
                 is_correct ? "right" : "wrong");
        
        A = 8'b01111111; B = 8'b10000000; #10;
        expected = calculate_expected(A, B);
        is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
        $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                 $time, A, B, 
                 expected[2], expected[1], expected[0],
                 A_gt_B, A_eq_B, A_lt_B,
                 is_correct ? "right" : "wrong");
        
        // 随机测试
        for(i = 0; i < 10; i = i + 1) begin
            A = $random;
            B = $random;
            #10;
            expected = calculate_expected(A, B);
            is_correct = ({A_gt_B, A_eq_B, A_lt_B} === expected);
            $display("%0t\t%b\t%b\t%d\t%d\t%d\t%d\t%d\t%d\t%s", 
                     $time, A, B, 
                     expected[2], expected[1], expected[0],
                     A_gt_B, A_eq_B, A_lt_B,
                     is_correct ? "right" : "wrong");
        end
        
        $finish;
    end
endmodule
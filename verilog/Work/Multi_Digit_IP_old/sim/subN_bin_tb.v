`timescale 1ns/1ps

module subN_bin_tb;
    parameter N = 8;
    
    reg  [N-1:0] A;
    reg  [N-1:0] B;
    reg  Bin;
    wire [N-1:0] Diff;
    wire Bout;

    // 实例化被测设计
    subN_bin #(.N(N)) uut (
        .A(A),
        .B(B),
        .Bin(Bin),
        .Diff(Diff),
        .Bout(Bout)
    );

    // 计算期望结果
    function [N:0] calculate_expected;
        input [N-1:0] a;
        input [N-1:0] b;
        input bin;
        begin
            calculate_expected = a - b - bin;
        end
    endfunction

    integer i;
    reg [N:0] expected;
    reg signed [N:0] signed_result;
    reg is_correct;
    
    initial begin
        i = 0;
        expected = 0;
        signed_result = 0;
        is_correct = 0;
        $display("Time(ns)\tA\t\tB\t\tBin\tDiff\t\tBout\tExcepted_Answer\tAnswer\tCorrect");
        $display("==========================================================================================");
        
        // 特定测试用例
        A = 8'b00000000; B = 8'b00000000; Bin = 0; #10;
        expected = calculate_expected(A, B, Bin);
        signed_result = A - B - Bin;
        is_correct = ({Bout, Diff} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%d\t\t%s", 
                 $time, A, B, Bin, Diff, Bout, expected, signed_result,
                 is_correct ? "right" : "wrong");
        
        A = 8'b00000001; B = 8'b00000001; Bin = 0; #10;
        expected = calculate_expected(A, B, Bin);
        signed_result = A - B - Bin;
        is_correct = ({Bout, Diff} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%d\t\t%s", 
                 $time, A, B, Bin, Diff, Bout, expected, signed_result,
                 is_correct ? "right" : "wrong");
        
        A = 8'b00000001; B = 8'b00000010; Bin = 0; #10;
        expected = calculate_expected(A, B, Bin);
        signed_result = A - B - Bin;
        is_correct = ({Bout, Diff} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%d\t\t%s", 
                 $time, A, B, Bin, Diff, Bout, expected, signed_result,
                 is_correct ? "right" : "wrong");
        
        A = 8'b11111111; B = 8'b00000001; Bin = 0; #10;
        expected = calculate_expected(A, B, Bin);
        signed_result = A - B - Bin;
        is_correct = ({Bout, Diff} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%d\t\t%s", 
                 $time, A, B, Bin, Diff, Bout, expected, signed_result,
                 is_correct ? "right" : "wrong");
        
        A = 8'b11111111; B = 8'b11111111; Bin = 1; #10;
        expected = calculate_expected(A, B, Bin);
        signed_result = A - B - Bin;
        is_correct = ({Bout, Diff} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%d\t\t%s", 
                 $time, A, B, Bin, Diff, Bout, expected, signed_result,
                 is_correct ? "right" : "wrong");
        
        A = 8'b10101010; B = 8'b01010101; Bin = 0; #10;
        expected = calculate_expected(A, B, Bin);
        signed_result = A - B - Bin;
        is_correct = ({Bout, Diff} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%d\t\t%s", 
                 $time, A, B, Bin, Diff, Bout, expected, signed_result,
                 is_correct ? "right" : "wrong");
        
        A = 8'b10101010; B = 8'b01010101; Bin = 1; #10;
        expected = calculate_expected(A, B, Bin);
        signed_result = A - B - Bin;
        is_correct = ({Bout, Diff} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%d\t\t%s", 
                 $time, A, B, Bin, Diff, Bout, expected, signed_result,
                 is_correct ? "right" : "wrong");
        
        // 随机测试
        for(i = 0; i < 10; i = i + 1) begin
            A = $random;
            B = $random;
            Bin = $random % 2;
            #10;
            expected = calculate_expected(A, B, Bin);
            signed_result = A - B - Bin;
            is_correct = ({Bout, Diff} === expected);
            $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%d\t\t%s", 
                     $time, A, B, Bin, Diff, Bout, expected, signed_result,
                     is_correct ? "right" : "wrong");
        end
        
        $finish;
    end
endmodule
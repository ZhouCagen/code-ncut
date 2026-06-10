`timescale 1ns/1ps

module addN_bin_tb;
    parameter N = 8;
    
    reg  [N-1:0] A;
    reg  [N-1:0] B;
    reg  Cin;
    wire [N-1:0] Sum;
    wire Cout;

    // 实例化被测设计
    addN_bin #(.N(N)) uut (
        .A(A),
        .B(B),
        .Cin(Cin),
        .Sum(Sum),
        .Cout(Cout)
    );

    // 计算期望结果
    function [N:0] calculate_expected;
        input [N-1:0] a;
        input [N-1:0] b;
        input cin;
        begin
            calculate_expected = a + b + cin;
        end
    endfunction

    integer i;
    reg [N:0] expected;
    reg is_correct;
    
    initial begin
        i = 0;
        expected = 0;
        is_correct = 0;
        $display("Time(ns)\tA\t\tB\t\tCin\tSum\t\tCout\tExpected\tCorrect");
        $display("================================================================================");
        
        // 特定测试用例
        A = 8'b00000000; B = 8'b00000000; Cin = 0; #10;
        expected = calculate_expected(A, B, Cin);
        is_correct = ({Cout, Sum} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%s", 
                 $time, A, B, Cin, Sum, Cout, expected, 
                 is_correct ? "right" : "wrong");
        
        A = 8'b11111111; B = 8'b00000001; Cin = 0; #10;
        expected = calculate_expected(A, B, Cin);
        is_correct = ({Cout, Sum} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%s", 
                 $time, A, B, Cin, Sum, Cout, expected, 
                 is_correct ? "right" : "wrong");
        
        A = 8'b11111111; B = 8'b11111111; Cin = 1; #10;
        expected = calculate_expected(A, B, Cin);
        is_correct = ({Cout, Sum} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%s", 
                 $time, A, B, Cin, Sum, Cout, expected, 
                 is_correct ? "right" : "wrong");
        
        A = 8'b10101010; B = 8'b01010101; Cin = 0; #10;
        expected = calculate_expected(A, B, Cin);
        is_correct = ({Cout, Sum} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%s", 
                 $time, A, B, Cin, Sum, Cout, expected, 
                 is_correct ? "right" : "wrong");
        
        A = 8'b10101010; B = 8'b01010101; Cin = 1; #10;
        expected = calculate_expected(A, B, Cin);
        is_correct = ({Cout, Sum} === expected);
        $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%s", 
                 $time, A, B, Cin, Sum, Cout, expected, 
                 is_correct ? "right" : "wrong");
        
        // 随机测试
        for(i = 0; i < 10; i = i + 1) begin
            A = $random;
            B = $random;
            Cin = $random % 2;
            #10;
            expected = calculate_expected(A, B, Cin);
            is_correct = ({Cout, Sum} === expected);
            $display("%0t\t%b\t%b\t%d\t%b\t%d\t%09b\t%s", 
                     $time, A, B, Cin, Sum, Cout, expected, 
                     is_correct ? "right" : "wrong");
        end
        
        $finish;
    end
endmodule
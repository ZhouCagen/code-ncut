`timescale 1ns/1ps

module subN_bcd_tb;
    parameter N = 8;
    
    reg  [N-1:0] A;
    reg  [N-1:0] B;
    reg  Bin;
    wire [N-1:0] Diff;
    wire Bout;

    // 实例化被测设计
    subN_bcd #(.N(N)) uut (
        .A(A),
        .B(B),
        .Bin(Bin),
        .Diff(Diff),
        .Bout(Bout)
    );

    // 辅助函数，将BCD值转换为十进制
    function [7:0] bcd_to_decimal;
        input [7:0] bcd;
        begin
            bcd_to_decimal = (bcd[7:4] * 10) + bcd[3:0];
        end
    endfunction
    
    // 计算真实十进制结果
    function signed [15:0] calculate_real_result;
        input [7:0] a_bcd;
        input [7:0] b_bcd;
        input bin;
        input [7:0] diff_bcd;
        input bout;
        reg [7:0] a_dec, b_dec, diff_dec;
        begin
            a_dec = bcd_to_decimal(a_bcd);
            b_dec = bcd_to_decimal(b_bcd);
            diff_dec = bcd_to_decimal(diff_bcd);
            
            // 计算真实结果
            if (bout) begin
                // 有借位，结果为负数
                // 实际结果 = -(100 - diff_dec)
                calculate_real_result = -((100 - diff_dec));
            end else begin
                // 无借位，结果为正数
                calculate_real_result = diff_dec;
            end
        end
    endfunction
    
    // 生成有效的BCD随机数
    task gen_bcd_random;
        output [7:0] bcd;
        reg [3:0] tens, ones;
        begin
            tens = $urandom_range(0, 9); // 十位数 (0-9)
            ones = $urandom_range(0, 9); // 个位数 (0-9)
            bcd = {tens, ones};
        end
    endtask

    integer i;
    reg [7:0] temp_a, temp_b;
    reg signed [15:0] real_result;
    
    initial begin
        i = 0;
        temp_a = 0;
        temp_b = 0;
        
        $display("Time(ns)\tA\tB\tBin\tDiff\tBout\tAnswer");
        $display("==================================================================");
        
        // 使用正确BCD值的测试用例
        A = 8'h34; B = 8'h12; Bin = 0; #10; // 34 - 12 = 22
        real_result = calculate_real_result(A, B, Bin, Diff, Bout);
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Bin, 
                 bcd_to_decimal(Diff), Bout, real_result);
        
        A = 8'h12; B = 8'h34; Bin = 0; #10; // 12 - 34 = -22
        real_result = calculate_real_result(A, B, Bin, Diff, Bout);
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Bin, 
                 bcd_to_decimal(Diff), Bout, real_result);
        
        A = 8'h50; B = 8'h25; Bin = 1; #10; // 50 - 25 - 1 = 24
        real_result = calculate_real_result(A, B, Bin, Diff, Bout);
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Bin, 
                 bcd_to_decimal(Diff), Bout, real_result);
        
        A = 8'h00; B = 8'h01; Bin = 0; #10; // 0 - 1 = -1
        real_result = calculate_real_result(A, B, Bin, Diff, Bout);
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Bin, 
                 bcd_to_decimal(Diff), Bout, real_result);
        
        A = 8'h99; B = 8'h99; Bin = 0; #10; // 99 - 99 = 0
        real_result = calculate_real_result(A, B, Bin, Diff, Bout);
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Bin, 
                 bcd_to_decimal(Diff), Bout, real_result);
        
        // 使用有效BCD数的随机测试
        for(i = 0; i < 10; i = i + 1) begin
            gen_bcd_random(temp_a);  // 生成有效的BCD随机数
            gen_bcd_random(temp_b);
            A = temp_a;
            B = temp_b;
            Bin = $urandom_range(0, 1);
            #10;
            real_result = calculate_real_result(A, B, Bin, Diff, Bout);
            $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                     $time, bcd_to_decimal(A), bcd_to_decimal(B), Bin, 
                     bcd_to_decimal(Diff), Bout, real_result);
        end
        
        $finish;
    end
endmodule
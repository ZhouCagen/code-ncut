`timescale 1ns/1ps

module addN_bcd_tb;
    parameter N = 8;
    
    reg  [N-1:0] A;
    reg  [N-1:0] B;
    reg  Cin;
    wire [N-1:0] Sum;
    wire Cout;

    // 实例化被测设计
    addN_bcd #(.N(N)) uut (
        .A(A),
        .B(B),
        .Cin(Cin),
        .Sum(Sum),
        .Cout(Cout)
    );

    // 辅助函数，将BCD值转换为十进制
    function [7:0] bcd_to_decimal;
        input [7:0] bcd;
        begin
            // 检查是否为有效BCD
            if((bcd[7:4] > 9) || (bcd[3:0] > 9)) begin
                $display("错误: 非BCD值 %h", bcd);
                bcd_to_decimal = 0;
            end else begin
                bcd_to_decimal = (bcd[7:4] * 10) + bcd[3:0];
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
    
    initial begin
        $display("Time(ns)\tA\tB\tCin\tSum\tCout\tDecimal_Answer");
        $display("================================================================");
        
        // 使用正确BCD值的测试用例
        A = 8'h12; B = 8'h34; Cin = 0; #10; // 12 + 34 = 46
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Cin, 
                 bcd_to_decimal(Sum), Cout, 
                 (Cout * 100) + bcd_to_decimal(Sum));
        
        A = 8'h45; B = 8'h55; Cin = 0; #10; // 45 + 55 = 100
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Cin, 
                 bcd_to_decimal(Sum), Cout, 
                 (Cout * 100) + bcd_to_decimal(Sum));
        
        A = 8'h99; B = 8'h01; Cin = 0; #10; // 99 + 1 = 100
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Cin, 
                 bcd_to_decimal(Sum), Cout, 
                 (Cout * 100) + bcd_to_decimal(Sum));
        
        A = 8'h99; B = 8'h99; Cin = 0; #10; // 99 + 99 = 198
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Cin, 
                 bcd_to_decimal(Sum), Cout, 
                 (Cout * 100) + bcd_to_decimal(Sum));
        
        // 特定测试用例
        A = 8'h16; B = 8'h16; Cin = 0; #10; // 16 + 16 = 32
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Cin, 
                 bcd_to_decimal(Sum), Cout, 
                 (Cout * 100) + bcd_to_decimal(Sum));
        
        A = 8'h63; B = 8'h63; Cin = 0; #10; // 63 + 63 = 126
        $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                 $time, bcd_to_decimal(A), bcd_to_decimal(B), Cin, 
                 bcd_to_decimal(Sum), Cout, 
                 (Cout * 100) + bcd_to_decimal(Sum));
        
        // 使用有效BCD数的随机测试
        for(i = 0; i < 10; i = i + 1) begin
            gen_bcd_random(temp_a);  // 生成有效的BCD随机数
            gen_bcd_random(temp_b);
            A = temp_a;
            B = temp_b;
            Cin = $urandom_range(0, 1);
            #10;
            $display("%0t\t%d\t%d\t%d\t%d\t%d\t%d", 
                     $time, bcd_to_decimal(A), bcd_to_decimal(B), Cin, 
                     bcd_to_decimal(Sum), Cout, 
                     (Cout * 100) + bcd_to_decimal(Sum));
        end
        
        $finish;
    end
endmodule
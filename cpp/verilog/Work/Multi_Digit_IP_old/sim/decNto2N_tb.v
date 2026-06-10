`timescale 1ns/1ps

module decNto2N_bin_tb;
    parameter N = 3; // 测试3-8译码器
    
    reg  [N-1:0] A;
    reg  En;
    wire [(2**N)-1:0] Y;

    // 实例化被测设计
    decNto2N_bin #(.N(N)) uut (
        .A(A),
        .En(En),
        .Y(Y)
    );

    // 计算期望结果
    function [(2**N)-1:0] calculate_expected;
        input [N-1:0] a;
        input en;
        integer i;
        begin
            calculate_expected = 0;
            if (en) begin
                for(i = 0; i < (2**N); i = i + 1) begin
                    if (i == a) begin
                        calculate_expected[i] = 1'b1;
                    end
                end
            end
        end
    endfunction

    integer i;
    reg [(2**N)-1:0] expected;
    reg is_correct;
    
    initial begin
        $display("Time(ns)\tA\tEn\tY\t\tExpected\tCorrect");
        $display("================================================================");
        
        // 测试使能信号
        En = 0;
        for(i = 0; i < (2**N); i = i + 1) begin
            A = i;
            #10;
            expected = calculate_expected(A, En);
            is_correct = (Y === expected);
            $display("%0t\t%b\t%d\t%b\t%b\t%s", 
                     $time, A, En, Y, expected,
                     is_correct ? "right" : "wrong");
        end
        
        // 测试所有输入组合
        En = 1;
        for(i = 0; i < (2**N); i = i + 1) begin
            A = i;
            #10;
            expected = calculate_expected(A, En);
            is_correct = (Y === expected);
            $display("%0t\t%b\t%d\t%b\t%b\t%s", 
                     $time, A, En, Y, expected,
                     is_correct ? "right" : "wrong");
        end
        
        $finish;
    end
endmodule
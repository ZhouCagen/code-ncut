`timescale 1ns / 1ps

module decimal_adder_tb;
    
    reg [3:0] A, B;
    reg Cin;
    wire [3:0] Sum;
    wire Cout;
    
    // 实例化 BCD 加法器
    decimal_adder uut (
        .A(A),
        .B(B),
        .Cin(Cin),
        .Sum(Sum),
        .Cout(Cout)
    );
    
    initial begin
        $display("=== BCD 加法器测试 ===");

        // 测试 7+5, Cin=0
        A = 4'd7; B = 4'd5; Cin = 0; #10;
        $display("%d + %d + Cin=%b => Sum=%d Cout=%b", A, B, Cin, Sum, Cout);

        // 测试 9+9, Cin=1
        A = 4'd9; B = 4'd9; Cin = 1; #10;
        $display("%d + %d + Cin=%b => Sum=%d Cout=%b", A, B, Cin, Sum, Cout);

        $stop; // 停止仿真
    end

endmodule
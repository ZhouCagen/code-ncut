`timescale 1ns / 1ps

module binary_adder_tb;
    
    reg [7:0] A, B;
    reg Cin;
    wire [7:0] Sum;
    wire Cout;
    
    // 实例化 8 位二进制加法器
    binary_adder #(8) uut (
        .A(A),
        .B(B),
        .Cin(Cin),
        .Sum(Sum),
        .Cout(Cout)
    );
    
    initial begin
        $display("=== 二进制加法器测试 ===");

        // 测试 5+10, Cin=0
        A = 8'd5; B = 8'd10; Cin = 0; #10;
        $display("%d + %d + Cin=%b => Sum=%d Cout=%b", A, B, Cin, Sum, Cout);

        // 测试 200+100, Cin=1
        A = 8'd200; B = 8'd100; Cin = 1; #10;
        $display("%d + %d + Cin=%b => Sum=%d Cout=%b", A, B, Cin, Sum, Cout);

        $stop; // 停止仿真
    end

endmodule
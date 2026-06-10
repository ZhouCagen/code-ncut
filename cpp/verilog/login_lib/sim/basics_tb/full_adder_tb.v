`timescale 1ns / 1ps

module full_adder_tb;

    reg A, B, Cin;
    wire Sum, Cout;

    // 实例化全加器
    full_adder uut (
        .A(A),
        .B(B),
        .Cin(Cin),
        .Sum(Sum),
        .Cout(Cout)
    );

    initial begin
        $display("=== 全加器测试 ===");
        // 遍历所有 A、B、Cin 组合 (000 ~ 111)
        for (integer j = 0; j < 8; j = j + 1) begin
            {A, B, Cin} = j;
            #5;
            $display("A=%b B=%b Cin=%b => Sum=%b Cout=%b", A, B, Cin, Sum, Cout);
        end
        $stop;
    end

endmodule

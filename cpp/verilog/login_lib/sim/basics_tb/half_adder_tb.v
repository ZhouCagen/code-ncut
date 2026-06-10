`timescale 1ns / 1ps

module half_adder_tb;

    reg A, B;
    wire Sum, Carry;

    // 实例化半加器
    half_adder uut (
        .A(A),
        .B(B),
        .Sum(Sum),
        .Carry(Carry)
    );

    initial begin
        $display("=== 半加器测试 ===");
        // 遍历所有 A、B 组合 (00, 01, 10, 11)
        for (integer i = 0; i < 4; i = i + 1) begin
            {A, B} = i;
            #5;
            $display("A=%b B=%b => Sum=%b Carry=%b", A, B, Sum, Carry);
        end
        $stop;
    end

endmodule

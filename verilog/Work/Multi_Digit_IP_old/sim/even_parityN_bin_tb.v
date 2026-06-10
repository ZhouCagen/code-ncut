`timescale 1ns/1ps

module even_parityN_bin_tb;

    // 参数化
    parameter N = 8;

    // 输入 / 输出
    reg  [N-1:0] A;
    wire P;

    // DUT 实例化
    even_parityN_bin #(.N(N)) uut (
        .A(A),
        .P(P)
    );

    // 参考值 (用内置 ^ 做比较)
    wire P_ref;
    assign P_ref = ~(^A);  // 偶校验: 确保总共有偶数个1

    integer k;

    initial begin
        k = 0;  // 初始化，避免波形里出现 X

        $display("==== Even Parity %0d-bit Test ====", N);

        // 固定测试
        A = 8'b0000_0000; #10;
        $display("A=%b P=%b (ref=%b)", A, P, P_ref);

        A = 8'b0000_0001; #10;
        $display("A=%b P=%b (ref=%b)", A, P, P_ref);

        A = 8'b1111_1111; #10;
        $display("A=%b P=%b (ref=%b)", A, P, P_ref);

        // 随机测试
        for (k=0; k<10; k=k+1) begin
            A = $random;
            #10;
            $display("A=%b P=%b (ref=%b)", A, P, P_ref);
            if (P !== P_ref) begin
                $display("ERROR: mismatch at A=%b", A);
            end
        end

        $display("==== Test Done ====");
        $stop;
    end

endmodule

`timescale 1ns/1ps

module mulN_bin_tb;

    parameter N = 8;

    reg  [N-1:0] A, B;
    wire [2*N-1:0] Product;

    // 实例化被测模块
    mulN_bin #(N) uut (
        .A(A),
        .B(B),
        .Product(Product)
    );

    integer i;

    initial begin
        // 打印表头
        $display("Time\tA\tB\tProduct\tExpected");
        $display("---------------------------------------------");

        // 测试固定值
        A = 8'd15; B = 8'd12; #10;
        $display("%0t\t%0d\t%0d\t%0d\t%0d", $time, A, B, Product, A*B);

        A = 8'd7; B = 8'd9; #10;
        $display("%0t\t%0d\t%0d\t%0d\t%0d", $time, A, B, Product, A*B);

        // 测试随机值
        for (i = 0; i < 10; i = i + 1) begin
            A = $random % (1<<N);
            B = $random % (1<<N);
            #10;
            $display("%0t\t%0d\t%0d\t%0d\t%0d", $time, A, B, Product, A*B);
        end

        $stop;
    end

endmodule

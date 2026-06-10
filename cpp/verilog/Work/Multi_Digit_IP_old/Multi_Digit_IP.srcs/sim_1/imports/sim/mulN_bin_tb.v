`timescale 1ns/1ps

module mulN_bin_tb;

    parameter N = 8;

    reg  [N-1:0] A;
    reg  [N-1:0] B;
    wire [2*N-1:0] Product;

    // DUT 实例化
    mulN_bin #(.N(N)) uut (
        .A(A),
        .B(B),
        .Product(Product)
    );

    integer i;

    initial begin
        $display("Time\tA\tB\tProduct\t(Expected)");

        // 测试几个固定值
        A = 8'd12; B = 8'd34;
        #10 $display("%0dns\t%0d\t%0d\t%0d\t%0d", $time, A, B, Product, A*B);

        A = 8'd255; B = 8'd2;
        #10 $display("%0dns\t%0d\t%0d\t%0d\t%0d", $time, A, B, Product, A*B);

        A = 8'd15; B = 8'd15;
        #10 $display("%0dns\t%0d\t%0d\t%0d\t%0d", $time, A, B, Product, A*B);

        // 随机测试 10 次
        for(i = 0; i < 10; i = i + 1) begin
            A = $random % 256;  // 0~255
            B = $random % 256;
            #10;
            $display("%0dns\t%0d\t%0d\t%0d\t%0d", $time, A, B, Product, A*B);
        end

        $finish;
    end

endmodule

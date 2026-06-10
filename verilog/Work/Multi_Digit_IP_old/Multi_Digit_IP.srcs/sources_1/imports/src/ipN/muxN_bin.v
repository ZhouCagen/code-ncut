module muxN_bin #(parameter N = 8)(
    input [N-1:0] A, B,
    input Sel,
    output [N-1:0] Y
);
    // 生成多个2位多路选择器实例
    genvar i;
    generate
        for (i = 0; i < N/2; i = i + 1) begin : mux_loop
            mux2_bin mux2 (
                .A(A[i*2+1:i*2]),
                .B(B[i*2+1:i*2]),
                .Sel(Sel),
                .Y(Y[i*2+1:i*2])
            );
        end
    endgenerate
endmodule
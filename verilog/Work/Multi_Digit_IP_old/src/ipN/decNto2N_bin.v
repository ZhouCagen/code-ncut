module decNto2N_bin #(parameter N = 8)(
    input wire [N-1:0] A,
    input wire En,
    output wire [(2**N)-1:0] Y
);

    // 使用迭代方法实现N-2^N译码器
    generate
        if (N == 1) begin
            // 基础情况：1-2译码器
            assign Y[0] = En & (~A[0]);
            assign Y[1] = En & A[0];
        end else if (N == 2) begin
            // 直接使用2-4译码器
            dec2to4_bin u_dec2to4(
                .A(A),
                .En(En),
                .Y(Y)
            );
        end else begin
            // 对于N>2，使用递归方法
            // 将问题分解为两个较小的译码器
            wire [(2**(N-1))-1:0] Y_low, Y_high;
            
            decNto2N_bin #(.N(N-1)) u_low (
                .A(A[N-2:0]),
                .En(En & (~A[N-1])),
                .Y(Y_low)
            );
            
            decNto2N_bin #(.N(N-1)) u_high (
                .A(A[N-2:0]),
                .En(En & A[N-1]),
                .Y(Y_high)
            );
            
            // 合并输出
            assign Y = {Y_high, Y_low};
        end
    endgenerate

endmodule
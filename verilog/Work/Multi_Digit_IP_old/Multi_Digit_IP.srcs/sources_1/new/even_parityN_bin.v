module even_parityN_bin #(parameter N = 8)(
    input  wire [N-1:0] A,
    output wire         P
);
    localparam integer PAIRS = (N + 1) / 2;

    wire [PAIRS-1:0] p_pair;
    genvar i;
    generate
        for (i = 0; i < PAIRS; i = i + 1) begin : parity_blocks
            wire [1:0] bits;
            // last block may be single bit -> pad MSB with 0
            assign bits = (2*i+1 < N) ? A[2*i +: 2] : {1'b0, A[2*i]};

            even_parity2_bin u_even_parity2 (
                .A(bits),
                .p(p_pair[i])
            );
        end
    endgenerate

    // 根据 PAIRS 的奇偶性选取合并方式（PAIRS 在编译期已知）
    generate
        if (PAIRS % 2 == 1) begin
            // odd number of pairs
            assign P = ^p_pair;       // reduction XOR of p_pair
        end else begin
            // even number of pairs
            assign P = ~(^p_pair);    // invert reduction XOR
        end
    endgenerate

endmodule

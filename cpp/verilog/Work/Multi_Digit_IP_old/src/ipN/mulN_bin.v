module mulN_bin #(parameter N = 8)(
    input  wire [N-1:0] A,
    input  wire [N-1:0] B,
    output wire [2*N-1:0] Product
);
    localparam M = (N + 1)/2;

    // 一维数组存储 mul2_bin 输出
    wire [3:0] mul2_out [0:M*M-1];

    // 一维数组存储拆分的 2 位块
    wire [1:0] A_block [0:M-1];
    wire [1:0] B_block [0:M-1];

    genvar i,j;
    generate
        for(i=0;i<M;i=i+1) begin : gen_A
            assign A_block[i] = (2*i+1 < N) ? A[2*i+1:2*i] : {1'b0, A[2*i]};
        end
        for(j=0;j<M;j=j+1) begin : gen_B
            assign B_block[j] = (2*j+1 < N) ? B[2*j+1:2*j] : {1'b0, B[2*j]};
        end
    endgenerate

    // 调用 mul2_bin
    generate
        for(i=0;i<M;i=i+1) begin : gen_mul2_i
            for(j=0;j<M;j=j+1) begin : gen_mul2_j
                mul2_bin u_mul2 (
                    .A(A_block[i]),
                    .B(B_block[j]),
                    .Product(mul2_out[i*M + j])
                );
            end
        end
    endgenerate

    // 累加结果
    reg [2*N-1:0] temp;
    integer m, n;
    always @* begin
        temp = 0;
        for(m = 0; m < M; m = m + 1) begin
            for(n = 0; n < M; n = n + 1) begin
                temp = temp + (mul2_out[m*M + n] << (2*m + 2*n));
            end
        end
    end

    assign Product = temp;

endmodule

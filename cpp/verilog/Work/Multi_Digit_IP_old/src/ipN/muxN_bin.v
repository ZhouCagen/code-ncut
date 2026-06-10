module muxN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    input wire Sel,
    output wire [N-1:0] Y
);

    genvar i;
    generate
        for(i = 0; i < N/2; i = i +1) begin
            wire [1:0] a_part = (2*i+1<N) ? A[2*i +:2] : {1'b0, A[2*i]};
            wire [1:0] b_part = (2*i+1<N) ? B[2*i +:2] : {1'b0, B[2*i]};   

            mux2_bin u_mux2_bin(
                .A(a_part),
                .B(b_part),
                .Sel(Sel),
                .Y(Y[2*i +: (2*i+1 < N ? 2:1)])
            );
        end    
    endgenerate
    
endmodule
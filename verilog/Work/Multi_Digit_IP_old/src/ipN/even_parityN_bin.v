module even_parityN_bin #(parameter N = 8)(
    input wire [N-1:0] A,
    output wire        P
);
    localparam integer PAIRS = (N+1)/2;
    
    wire [PAIRS-1:0] p_pair;
    
    genvar i;
    generate 
        for(i =0; i<PAIRS; i = i + 1) begin
            wire [1:0] bits;
            assign bits = (2*i +1<N) ? A[2*i +:2] : {1'b0, A[2*i]}; 
            
            even_parityN_bin u_even_parityN_bin(
                .A(bits),
                .P(p_pair[i])
            );
        end    
    endgenerate
    
    assign P = &p_pair ? 1'b1 : (^(p_pair)?1'b0 : 1'b1);
    
endmodule 

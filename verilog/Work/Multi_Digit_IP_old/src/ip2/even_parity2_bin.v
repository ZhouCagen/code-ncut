module even_parity2_bin(
    input  wire [1:0] A,
    output wire p
);

    assign p = ~(A[0] ^ A[1]);  
endmodule

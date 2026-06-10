module mulN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    input wire [2*N-1:0] Product
);

    assign Product = A * B;

endmodule
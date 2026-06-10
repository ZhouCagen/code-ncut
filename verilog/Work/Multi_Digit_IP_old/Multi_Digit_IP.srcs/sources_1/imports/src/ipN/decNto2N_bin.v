module decNto2N_bin #(parameter N = 8)(
    input wire [N-1:0] A,
    output wire [(2**N)-1:0] Y
);

    assign Y = 1 << A;

endmodule
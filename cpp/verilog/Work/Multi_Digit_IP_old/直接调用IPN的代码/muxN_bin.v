module muxN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    input wire Sel,
    output wire [N-1:0] Y
);

    assign Y = Sel ? B : A;

endmodule
module subN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    input wire Bin,
    output wire [N-1:0] Diff,
    output wire Bout
);

    assign {Bout, Diff} = A - B - Bin;

endmodule
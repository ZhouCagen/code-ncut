module sub2_bin(
    input wire [1:0] A, B,
    input wire Bin,
    output wire [1:0] Diff,
    output wire Bout
);
    
    assign {Bout, Diff} = A - B - Bin;

endmodule
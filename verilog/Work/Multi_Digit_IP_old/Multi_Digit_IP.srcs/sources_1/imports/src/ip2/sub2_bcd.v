module sub2_bcd(
    input wire [3:0] A,
    input wire [3:0] B,
    input wire Bin,
    output wire [3:0] Diff,
    output Bout
);

    wire [4:0] temp;
    assign temp = {1'b0, A} - B - Bin;
    assign {Bout, Diff} = (temp[4] == 1'b1) ? (temp + 5'd10) : temp;

endmodule
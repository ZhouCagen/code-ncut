module dec2to4_bin(
    input wire [1:0] A,
    input wire En,
    output wire [3:0] Y
);

    assign Y[0] = En & (~A[1]) & (~A[0]);
    assign Y[1] = En & (~A[1]) & A[0];
    assign Y[2] = En & A[1] & (~A[0]);
    assign Y[3] = En & A[1] & A[0];

endmodule
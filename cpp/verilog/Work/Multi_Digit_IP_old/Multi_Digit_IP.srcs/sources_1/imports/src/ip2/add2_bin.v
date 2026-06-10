module add2_bin(
    input wire [1:0] A, B,
    input wire Cin,
    output wire [1:0] Sum,
    output wire Cout
);

    assign {Cout, Sum} = A + B + Cin;

endmodule
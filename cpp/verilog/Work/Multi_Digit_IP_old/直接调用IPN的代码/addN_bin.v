module addN #(parameter N = 8)(
    input  [N-1:0] A, B,
    input  Cin,
    output [N-1:0] Sum,
    output Cout
);
    assign {Cout, Sum} = A + B + Cin;
endmodule

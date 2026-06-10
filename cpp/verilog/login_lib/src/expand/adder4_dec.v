module adder4_dec(
    input  wire [3:0] A3, A2, A1, A0,
    input  wire [3:0] B3, B2, B1, B0,
    input  wire       Cin,
    output wire [3:0] Sum3, Sum2, Sum1, Sum0,
    output wire       Cout
);

    wire c1, c2, c3;
    decimal_adder da0 (.A(A0), .B(B0), .Cin(Cin), .Sum(Sum0), .Cout(c1));
    decimal_adder da1 (.A(A1), .B(B1), .Cin(c1),  .Sum(Sum1), .Cout(c2));
    decimal_adder da2 (.A(A2), .B(B2), .Cin(c2),  .Sum(Sum2), .Cout(c3));
    decimal_adder da3 (.A(A3), .B(B3), .Cin(c3),  .Sum(Sum3), .Cout(Cout));

endmodule
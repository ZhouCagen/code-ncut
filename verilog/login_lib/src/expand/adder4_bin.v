module adder4_bin(
    input  wire [3:0] A,
    input  wire [3:0] B,
    input  wire       Cin,
    output wire [3:0] Sum,
    output wire       Cout
);

wire c1, c2, c3;

full_adder fa0 (.A(A[0]), .B(B[0]), .Cin(Cin), .Sum(Sum[0]), .Cout(c1));
full_adder fa1 (.A(A[1]), .B(B[1]), .Cin(c1) , .Sum(Sum[1]), .Cout(c2));
full_adder fa2 (.A(A[2]), .B(B[2]), .Cin(c2) , .Sum(Sum[2]), .Cout(c3));
full_adder fa3 (.A(A[3]), .B(B[3]), .Cin(c3) , .Sum(Sum[3]), .Cout(Cout));

endmodule
module mux4(
    input  wire I0,
    input  wire I1,
    input  wire I2,
    input  wire I3,
    input  wire [1:0] S,
    output wire Y
);

    wire M0,M1;
    
    mux2to1 m1(.A(I0), .B(I1), .Sel(S[0]), .Y(M0));
    mux2to1 m2(.A(I2), .B(I3), .Sel(S[0]), .Y(M1));
    mux2to1 m (.A(M0), .B(M1), .Sel(S[1]), .Y(Y));
    

endmodule
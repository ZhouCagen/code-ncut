module add2_bcd(
    input wire [3:0] A, B,
    input wire Cin,
    output wire [3:0] Sum,
    output wire Cout
);
    
    wire [4:0] temp;
    assign temp = A + B + Cin;
    assign {Cout, Sum} = (temp > 9) ? (temp - 10) : temp;
    
endmodule
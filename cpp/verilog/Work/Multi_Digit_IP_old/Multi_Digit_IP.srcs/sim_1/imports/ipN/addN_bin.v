module addN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    input wire Cin,
    output wire [N-1:0] Sum,
    output wire Cout
);
    
    assign {Cout, Sum} = A + B + Cin;
    
endmodule
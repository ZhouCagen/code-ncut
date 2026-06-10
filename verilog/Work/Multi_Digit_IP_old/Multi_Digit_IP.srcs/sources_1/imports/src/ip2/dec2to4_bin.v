module dec2to4_bin(
    input wire [1:0] A,
    output wire [3:0] Y
);

    assign Y = 1 << A;
    
endmodule    
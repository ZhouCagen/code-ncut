 module mux2_bin(
    input [1:0] A, B,
    input Sel,
    output [1:0] Y
 );
 
    assign Y = Sel ? B : A;
    
 endmodule
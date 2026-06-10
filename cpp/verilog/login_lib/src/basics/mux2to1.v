module mux2to1(
    input wire A,
    input wire B,
    input wire Sel,
    output wire Y
);

    assign Y = (Sel) ? B : A; //当 sel=0 输出 A ，sel=0 输出 B
    
endmodule 
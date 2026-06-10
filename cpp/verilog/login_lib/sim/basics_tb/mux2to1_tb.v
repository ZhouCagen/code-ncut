`timescale 1ns / 1ps

module mux2to1_tb;
    
    reg A, B, Sel;
    wire Y;
    
    mux2to1 uut (
        .A(A),
        .B(B),
        .Sel(Sel),
        .Y(Y)
    );
    
    initial begin
        $display("=== 2选1数选器测试===");
        $display(" Sel A B | Y");
        
        for( integer i = 0; i < 8; i = i + 1) begin
            {Sel, A, B} = i;
            #5;
            $display(" %b %b %b | %b",Sel,A,B,Y);
        end
        
        $stop;
     end
     
endmodule
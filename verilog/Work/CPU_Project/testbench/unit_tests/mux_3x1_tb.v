`timescale 1ns/1ps

module mux_3x1_tb();

    reg  [31:0] in0_tb;
    reg  [31:0] in1_tb;
    reg  [31:0] in2_tb;
    reg  [1:0]  sel_tb;
    wire [31:0] out_tb;

    mux_3x1 #(.Width(32)) dut(
        .in0(in0_tb),
        .in1(in1_tb),
        .in2(in2_tb),
        .sel(sel_tb),
        .out(out_tb)
    );

    initial begin

        in0_tb = 32'hAAAA_AAAA;
        in1_tb = 32'hBBBB_BBBB;
        in2_tb = 32'hCCCC_CCCC;
        
        sel_tb = 2'b00;
        #10;
        $display("SEL=%b -> OUT = %h (Expected: AAAAAAAA)", sel_tb, out_tb);
        
        sel_tb = 2'b01;
        #10;
        $display("SEL=%b -> OUT = %h (Expected: BBBBBBBB)", sel_tb, out_tb);
        
        sel_tb = 2'b10;
        #10;
        $display("SEL=%b -> OUT = %h (Expected: CCCCCCCC)", sel_tb, out_tb);
        
        sel_tb = 2'b11;
        #10;
        $display("SEL=%b -> OUT = %h (Expected: AAAAAAAA)", sel_tb, out_tb);
        
        in0_tb = 32'h1111_1111;
        in1_tb = 32'h2222_2222;
        in2_tb = 32'h3333_3333;
        #5;
        $display("After input change, SEL=%b -> OUT = %h (Should be unchanged)", sel_tb, out_tb);
        
        sel_tb = 2'b01; 
        #10;
        $display("SEL=%b -> OUT = %h (Expected: 22222222)", sel_tb, out_tb);
        
        #10;
        $finish;
    end

endmodule
`timescale 1ns/1ps

module mux_2x1_tb();

    reg [31:0]  in0_tb;
    reg [31:0]  in1_tb;
    reg         sel_tb;
    wire [31:0] out_tb;

    mux_2x1 #(.Width(32)) dut(
        .in0(in0_tb),
        .in1(in1_tb),
        .sel(sel_tb),
        .out(out_tb)
    );

    initial begin
        in0_tb = 32'hAAAA_AAAA;
        in1_tb = 32'h5555_5555;
        sel_tb = 1'b0;

        #10;

        sel_tb = 1'b0;
        $display("Test Case 1 - SEL=0: OUT = %h", out_tb);

        sel_tb = 1'b1;
        #10;
        $display("Test Case 2 - SEL=1: OUT = %h", out_tb);

        in0_tb = 32'hDEAD_BEEF;
        in1_tb = 32'hCAFE_BABE;
        #10;
        $display("Test Case 3 - SEL=1, IN changed: OUT = %h", out_tb);

        sel_tb = 1'b0;
        #10;
        $display("Test Case 4 - SEL=0, IN changed: OUT = %h", out_tb);

        $finish;

    end
    
endmodule    
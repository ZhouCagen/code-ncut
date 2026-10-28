`timescale 1ns/1ps

module d_flip_flop_tb();

    reg          clk_tb;
    reg          rst_tb;
    reg [31:0]   D_tb;
    wire [31:0]  Q_tb;

    d_flip_flop #(.Width(32)) dut(
        .clk(clk_tb),
        .rst(rst_tb),
        .D(D_tb),
        .Q(Q_tb)
    );

    always #10 clk_tb = ~clk_tb; 

    initial begin

        clk_tb = 1'b0;
        rst_tb = 1'b0; 
        D_tb   = 32'd0; 

        #15;
        rst_tb = 1'b1; 
        #20;
        rst_tb = 1'b0; 
        #15;

        D_tb = 32'hAAAA_AAAA;
        #20;

        D_tb = 32'h5555_5555;
        #5;
        D_tb = 32'hDEAD_BEEF;
        #15;

        #7;
        rst_tb = 1'b1;
        #3;
        rst_tb = 1'b0;
        #20;

        $finish;
    end

    initial begin
        $monitor("Time=%0tns CLK=%b RST=%b D=%h Q=%h",$time, clk_tb, rst_tb, D_tb, Q_tb);
    end

endmodule
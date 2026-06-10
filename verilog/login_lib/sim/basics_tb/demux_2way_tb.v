`timescale 1ns / 1ps

module demux_2way_tb;

    reg D;
    reg sel;
    wire Y0, Y1;
    
    demux_2way uut(
        .D(D),
        .sel(sel),
        .Y0(Y0),
        .Y1(Y1)
    );  
    
    initial begin
        D = 0;
        sel = 0;
        $monitor("Time=%0t | D=%b sel=%b | Y0=%b Y1=%b", $time, D, sel, Y0, Y1);

        #10 D=0; sel=1;
        #10 D=1; sel=0;
        #10 D=1; sel=1;
        #10 D=0; sel=0;

        #10 $finish; 
    end

endmodule
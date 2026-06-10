module demux4(
    input  wire D,
    input  wire [1:0] sel,
    output wire Y0,
    output wire Y1,
    output wire Y2,
    output wire Y3
);
    
    wire d_low,d_high;
    
    demux_2way d0(.D(D), .sel(sel[1]), .Y0(d_low), .Y1(d_high));
    demux_2way d1(.D(d_low) , .sel(sel[0]), .Y0(Y0), .Y1(Y1));
    demux_2way d2(.D(d_high), .sel(sel[0]), .Y0(Y2), .Y1(Y3));
    
endmodule 
`timescale 1ns/1ps

module mux_2x1 #(parameter Width = 32)(
    input wire  [Width-1:0] in0,
    input wire  [Width-1:0] in1,
    input wire              sel,    
    output wire [Width-1:0] out
);

assign out = (sel) ? in1 : in0;

endmodule
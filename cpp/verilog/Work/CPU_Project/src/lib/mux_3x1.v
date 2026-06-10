`timescale 1ns/1ps

module mux_3x1 #(parameter Width = 32)(
    input wire  [Width-1:0] in0,
    input wire  [Width-1:0] in1,
    input wire  [Width-1:0] in2,
    input wire  [1:0]       sel,
    output wire [Width-1:0] out
);

assign out = (sel == 2'b00) ? in0 : ((sel == 2'b01) ? in1 : ((sel == 2'b10) ? in2 : in0));

endmodule
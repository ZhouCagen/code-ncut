`timescale 1ns/1ps

module d_flip_flop #(parameter Width= 32)(
    input wire clk,
    input wire rst,
    input wire [Width-1:0] D,
    output reg [Width-1:0] Q 
);

    always @(posedge clk or posedge rst) begin
        if(rst) begin
            Q <= {Width{1'b0}};
        end else begin
            Q <= D;
        end
    end

endmodule    

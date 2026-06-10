module bcd_encoder(
    input wire [9:0] in,
    output reg [3:0] out
);
    always @(*) begin
        case(in)
            10'b0000000001: out = 4'd0;
            10'b0000000010: out = 4'd1;
            10'b0000000100: out = 4'd2;
            10'b0000001000: out = 4'd3;
            10'b0000010000: out = 4'd4;
            10'b0000100000: out = 4'd5;
            10'b0001000000: out = 4'd6;
            10'b0010000000: out = 4'd7;
            10'b0100000000: out = 4'd8;
            10'b1000000000: out = 4'd9;
            default:        out = 4'd0;
        endcase
    end
    
endmodule
module seg_scan(
    input  wire clk,
    input  wire rst,
    input  wire [255:0] char_data,   // 整个字模
    input  wire common_anode,
    input  wire enable,
    output reg  [15:0] row_data,      // 只输出行数据
    output reg  [3:0] row_index
);

    reg [11:0] div = 0;
    always @(posedge clk) div = div + 1;
    always @(div[11:8]) row_index = div[11:8];
    
    reg [15:0] raw_row;
    always @(*) begin
        raw_row = char_data[16*(15-row_index) +: 16];
    end
    
    always @(posedge clk) begin
        if (!enable) begin
            if (common_anode) row_data <= 16'hFFFF;
            else              row_data <= 16'h0000;
        end else begin
            if (common_anode) row_data <= ~raw_row;
            else               row_data <= raw_row;
        end
    end 

endmodule
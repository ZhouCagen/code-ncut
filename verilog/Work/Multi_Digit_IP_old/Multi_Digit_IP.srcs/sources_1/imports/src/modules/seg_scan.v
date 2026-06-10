module seg_scan(
    input  wire [15:0] clk_div_cnt,
    input  wire rst,
    input  wire [255:0] char_data,   // 整个字模
    input  wire common_anode,
    output reg  [15:0] row_data      // 只输出行数据
);

    reg [3:0] row_index;
    wire scan_clk = clk_div_cnt[4];

    always @(posedge scan_clk or posedge rst) begin
        if(rst)
            row_index <= 0;
        else
            row_index <= row_index + 1;
    end

    always @(*) begin
        if(common_anode)  // [start +: width] 表示从 start 位开始，向高位取 width 位
            row_data = ~char_data[16*(15-row_index) +: 16];
        else
            row_data =  char_data[16*(15-row_index) +: 16];
    end       

endmodule
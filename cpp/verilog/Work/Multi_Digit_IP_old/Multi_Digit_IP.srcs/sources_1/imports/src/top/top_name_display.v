module top_name_display(
    input  wire clk,
    input  wire rst,
    input  wire [1:0] char_index,
    input  wire common_anode,
    output wire [15:0] row_data
);

    wire [15:0] clk_div_cnt;
    clk_div u_clk_div(
        .clk(clk),
        .rst(rst),
        .clk_div_cnt(clk_div_cnt)
    );

    wire [255:0] char_data;
    char_rom u_char_rom(
        .char_index(char_index),
        .char_data(char_data)
    );

    seg_scan u_seg_scan(
        .clk_div_cnt(clk_div_cnt),
        .rst(rst),
        .char_data(char_data),
        .common_anode(common_anode),
        .row_data(row_data)
    );

endmodule
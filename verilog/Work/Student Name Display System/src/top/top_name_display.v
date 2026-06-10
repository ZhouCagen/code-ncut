module top_name_display(
    input  wire        clk,
    input  wire        rst,
    input  wire [1:0]  char_index,
    input  wire        auto_mode,
    input  wire        common_anode,
    output wire [15:0] row_data,
    output wire [3:0]  row_idx
);
    reg [31:0] div = 0;
    always @(posedge clk) div <= div + 1;
    
    reg [1:0] auto_sel;
    always @(*) begin
        case(div[23:22])
            2'b00: auto_sel = 2'd0;
            2'b01: auto_sel = 2'd1;
            2'b10: auto_sel = 2'd2;
            2'b11: auto_sel = 2'd0;
            default: auto_sel = 2'd0;
        endcase
    end          
    
    // 选择给 ROM 的索引：自动模式用 auto_sel，手动模式用 char_index
    wire [1:0] rom_idx;
    assign rom_idx = (auto_mode ? auto_sel : char_index);
    
    // 手动模式时要闪烁：用 div[20] 作为闪烁开关（1 = 显示，0 = 关闭）
    // 自动模式始终显示（1）
    
    wire char_enable;
    assign char_enable = (auto_mode ? 1'b1 : div[20]);
    
    wire [255:0] char_data;
    
    char_rom u_char_rom(
        .char_index(rom_idx),
        .char_data(char_data)
    );

    seg_scan u_seg_scan(
        .clk(clk),
        .rst(rst),
        .char_data(char_data),
        .common_anode(common_anode),
        .enable(char_enable),
        .row_data(row_data),
        .row_index(row_idx)
    );
    
endmodule
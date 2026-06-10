`timescale 1ns / 1ps

module top(
    input  wire       clk,    // 时钟信号，所有寄存器同步于此时钟
    output reg  [2:0] pos,    // 数码管扫描位置控制，高三位表示当前扫描位
    output reg  [6:0] seg     // 七段数码管显示输出
);

    // 分频计数器：用于产生慢于系统时钟的扫描频率
    reg [8:0] div;
    always @(posedge clk) begin
        div <= div + 1'b1;     // 每个时钟上升沿加1，实现计数
    end

    // 扫描控制寄存器：循环扫描3位数码管
    reg [1:0] scan;
    always @(posedge clk) begin
        if (div == 9'd0)       // 分频计满时更新扫描位
            scan <= (scan == 2) ? 0 : scan + 1'b1; // 0→1→2→0循环
    end

    // 将扫描位转换为实际数码管控制信号
    always @(posedge clk) begin
        pos <= {1'b0, scan};   // 高位填0，低两位为扫描索引
    end

    // BCD值选择：根据扫描位决定显示的数字
    reg [3:0] BCD;
    always @(*) begin
        case (scan)
            2'b00: BCD = 4'd2;   // 第一位显示数字2
            2'b01: BCD = 4'd2;   // 第二位显示数字1
            2'b10: BCD = 4'd2;   // 第三位显示数字0
            default: BCD = 4'd0; // 默认值0，避免不确定状态
        endcase
    end

    // 七段数码管译码逻辑：将BCD码转换为段选信号
    always @(*) begin
        case (BCD)
            4'd0: seg = 7'b1111110;
            4'd1: seg = 7'b0110000;
            4'd2: seg = 7'b1101101;
            4'd3: seg = 7'b1111001;
            4'd4: seg = 7'b0110011;
            4'd5: seg = 7'b1011011;
            4'd6: seg = 7'b1011111;
            4'd7: seg = 7'b1110000;
            4'd8: seg = 7'b1111111;
            4'd9: seg = 7'b1111011;
            default: seg = 7'b0000000; // 默认灭灯，避免悬空
        endcase
    end

endmodule

`timescale 1ns / 1ps

module top_tb;

    reg clk;             // 测试时钟信号
    wire [2:0] pos;      // 数码管位选
    wire [6:0] seg;      // 数码管段选

    // 例化待测模块
    top uut (
        .clk(clk),
        .pos(pos),
        .seg(seg)
    );

    // 产生时钟：周期 10ns（100MHz）
    initial clk = 0;
    always #5 clk = ~clk;

    // 仿真过程
    initial begin
        // 仿真时间
        $display("Start Simulation...");
        #2000;   // 仿真 2000ns
        $display("End Simulation.");
        $stop;
    end

endmodule

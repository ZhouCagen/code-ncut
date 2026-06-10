`timescale 1ns/1ps
module top_student_name_tb();

    reg clk;
    reg rst;
    reg [1:0] char_index;
    reg common_anode;
    
    wire [15:0] row_data; 
    
    // 实例化你的顶层模块
    top_name_display uut(
        .clk(clk),
        .rst(rst),
        .char_index(char_index),
        .common_anode(common_anode),
        .row_data(row_data)
    );
    
    // 时钟 50 MHz
    initial clk = 0;
    always #10 clk = ~clk; // 20 ns 一个周期
    
    // 初始复位
    initial begin
        rst = 1;
        char_index = 0;
        common_anode = 0; 
        #50;
        rst = 0;
    end
    
    // 每隔一段时间切换字符
    initial begin
        repeat (3) begin
            #500; // 500 ns 切换
            char_index = char_index + 1;
        end    
    end
    
    // 仿真停止
    initial begin
        #5000;
        $stop;
    end
    
endmodule

`timescale 1ns/1ps

module counter2_tb;

    reg clk;
    reg rst;
    wire [1:0] Q;
    
    counter2 uut(
        .clk(clk),
        .rst(rst),
        .Q(Q)
    );
   
    initial clk = 0;
    always #5 clk = ~clk;
    initial begin
          $display("=== 2 位计数器测试 ===");
          $display("Time(ns) | rst | Q1 Q0");
          $display("------------------------");
          
          //初始化复位
          rst = 1; #10;
          rst = 0; #10;
          
          repeat (10) #10 $display("%8t |  %b  | %b %b", $time, rst, Q[1], Q[0]);
          
          rst = 1; #10;
          rst = 0; #10;
          $display("%8t |  %b  | %b %b (复位后计数)", $time, rst, Q[1], Q[0]);
          $display("=== 测试结束 ===");
          $finish;
    end
    
endmodule
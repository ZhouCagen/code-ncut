`timescale 1ns / 1ps

module bcd_decoder_tb;
    reg [3:0]  bcd;
    reg        en;
    wire [9:0] out;
    
    bcd_decoder uut(
        .bcd(bcd),
        .en(en),
        .out(out)
    );
    
    initial begin
        $display("=== 二-十进制 (BCD) 译码器测试 ===");
        $display("time\t en bcd -> out");

        en = 0; bcd = 4'b0000; #10;   
        en = 1; bcd = 4'b0000; #10;   
        bcd = 4'b0001; #10;           
        bcd = 4'b0010; #10;           
        bcd = 4'b0011; #10;           
        bcd = 4'b0100; #10;           
        bcd = 4'b0101; #10;           
        bcd = 4'b0110; #10;           
        bcd = 4'b0111; #10;           
        bcd = 4'b1000; #10;          
        bcd = 4'b1001; #10;           
        bcd = 4'b1010; #10;           
        en = 0; #10;

        $display("=== 测试结束 ===");
        $finish;
    end
    
endmodule
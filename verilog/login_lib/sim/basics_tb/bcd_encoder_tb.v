`timescale 1ns / 1ps

module bcd_encoder_tb;
    
    reg [9:0] in;
    wire [4:0] out;
    
    bcd_encoder uut(
        .in(in),
        .out(out)
    );

    initial begin
        $display("=== 10-to-4 BCD 编码器测试 ===");
        
        for(integer i = 0; i < 10; i = i + 1) begin
            in=10'b0000000001 << i;
            #10;
            $display("%b => %b", in, out);
        end
        
        $display("\n=== 测试非法输入 ===");
        in = 10'b0000000011; #10; $display("%b => %b", in, out); // 多位高
        in = 10'b0000000000; #10; $display("%b => %b", in, out); // 全低
        
        $stop;
    end
    
endmodule
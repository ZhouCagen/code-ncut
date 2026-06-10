`timescale 1ns/1ps
module priority_encoder_tb;

    reg [7:0]  in;
    wire [2:0] out;
    wire       valid;
    
    priority_encoder uut(
        .in(in),
        .out(out),
        .valid(valid)
    );
    
    initial begin
        $display("=== 8-to-3 优先编码器 测试开始 ===");
        $display("time\t in        -> out valid");
        $display("--------------------------------");
        
        in = 8'b0000_0000; #10;
        in = 8'b0000_0001; #10;
        in = 8'b0000_0010; #10;
        in = 8'b0001_0000; #10;
        in = 8'b0100_0000; #10;
        in = 8'b1000_0000; #10;
        in = 8'b1111_1111; #10;
        in = 8'b0010_0000; #10;
        in = 8'b0000_0000; #10;
        
        $display("=== 测试结束 ===");
        
        $stop;
    end
endmodule
`timescale 1ns / 1ps

module binary_encoder_tb;

    reg [7:0] in;
    wire [2:0] out;
    
    binary_encoder uut(
        .in(in),
        .out(out)
    );
    
        
   initial begin
        $display("=== 8-to-3 二进制编码器测试===");
        $display("in       => out");
        for (integer i = 0; i < 8; i = i + 1) begin
            in = 8'b00000001 << i; // 逐位左移生成单高电平
            #10;
            $display("%b => %b", in, out);
        end
        
        $display("\n=== 测试非法输入 ===");
        in = 8'b00000110; #10; $display("%b => %b", in, out);
        in = 8'b00000000; #10; $display("%b => %b", in, out);
        
        $stop;
    end

endmodule
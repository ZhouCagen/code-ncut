`timescale 1ns / 1ps

module binary_decoder_tb;
    reg [2:0]  in;
    reg        en;
    wire [7:0] out;
    
    binary_decoder uut(
        .in(in),
        .en(en),
        .out(out)
    );
    
    initial begin
        $display("=== 3-to-8 二进制译码器测试 ===");
        $display("time\t en in -> out");
        
        en = 0; in = 3'b000; #10;   
        en = 1; in = 3'b000; #10;
        in = 3'b001; #10;
        in = 3'b010; #10;
        in = 3'b011; #10;
        in = 3'b100; #10;
        in = 3'b101; #10;
        in = 3'b110; #10;
        in = 3'b111; #10;
        en = 0; #10;
        $display("=== 测试结束 ===");
        
        $stop;
        
     end   
         
endmodule
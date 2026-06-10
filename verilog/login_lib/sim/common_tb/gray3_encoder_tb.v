`timescale 1ns/1ps
module gray3_encoder_tb;
    reg [2:0] bin;
    wire [2:0] gray;
    
    gray3_encoder uut(
        .bin(bin),
        .gray(gray)
    );
    
    initial begin
        $display("=== 3位二进制 -> Gray 编码测试 ===");
        $display("BIN | GRAY");
        $display("------------");

        for (bin = 0; bin < 8; bin = bin + 1) begin
            #10 $display(" %b  | %b", bin, gray);
        end

        $display("=== 测试结束 ===");
        $finish;
    end
    
endmodule 
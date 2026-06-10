`timescale 1ns/1ps

module seg7_decoder_tb;

    reg [3:0] BCD;
    wire [6:0] SEG;
    
    seg7_decoder uut(
        .BCD(BCD),
        .SEG(SEG)
    );
    
    initial begin
        $display("=== BCD -> 7段显示测试 ===");
        $display("BCD | SEG");
        $display("------------");

        // 测试 0~9
        for (BCD = 0; BCD <= 9; BCD = BCD + 1) begin
            #10 $display(" %b  | %b", BCD, SEG);
        end

        $display("=== 测试结束 ===");
        $finish;
        
     end
        
endmodule
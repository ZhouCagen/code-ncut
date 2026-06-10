`timescale 1ns/1ps

module demux4_tb;

    reg D;
    reg [1:0] sel;
    wire Y0, Y1, Y2, Y3;
    
    demux4 utt(
        .D(D),
        .sel(sel),
        .Y0(Y0), .Y1(Y1), .Y2(Y2), .Y3(Y3)
    );
    
    initial begin
        $display("=== 4 路数据分配器测试 ===");
        $monitor("Time=%0t D=%b sel=%b | Y0=%b Y1=%b Y2=%b Y3=%b",
                 $time, D, sel, Y0, Y1, Y2, Y3);

        D = 1; sel = 2'b00; #10;
        D = 1; sel = 2'b01; #10;
        D = 1; sel = 2'b10; #10;
        D = 1; sel = 2'b11; #10;

        D = 0; sel = 2'b00; #10;
        $stop;
    end
    
endmodule    
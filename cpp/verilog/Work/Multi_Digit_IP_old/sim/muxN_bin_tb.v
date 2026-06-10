`timescale 1ns/1ps

module muxN_bin_tb;
    // 测试参数
    parameter N = 8;
    parameter TEST_COUNT = 10;
    parameter CLK_PERIOD = 10;
    
    // 测试信号
    reg [N-1:0] A, B;
    reg Sel;
    wire [N-1:0] Y;
    
    // 时钟生成（用于随机测试）
    reg clk;
    always #(CLK_PERIOD/2) clk = ~clk;
    
    // 实例化被测设计
    muxN_bin #(.N(N)) uut (
        .A(A),
        .B(B),
        .Sel(Sel),
        .Y(Y)
    );
    
    // 测试过程
    initial begin
        // 初始化
        clk = 0;
        A = 0;
        B = 0;
        Sel = 0;
        
        // 测试1: Sel=0时选择A
        $display("Test 1: Sel=0, should output A");
        A = 8'b10101010;
        B = 8'b01010101;
        Sel = 0;
        #20;
        if (Y !== A) 
            $display("ERROR: Expected %b, Got %b", A, Y);
        else
            $display("PASS: Output matches A");
        
        // 测试2: Sel=1时选择B
        $display("Test 2: Sel=1, should output B");
        Sel = 1;
        #20;
        if (Y !== B) 
            $display("ERROR: Expected %b, Got %b", B, Y);
        else
            $display("PASS: Output matches B");
        
        // 测试3: 随机测试
        $display("Test 3: Random tests");
        for (integer i = 0; i < TEST_COUNT; i = i + 1) begin
            A = $random;
            B = $random;
            Sel = $random % 2;
            #20;
            
            if ((Sel === 0 && Y !== A) || (Sel === 1 && Y !== B))
                $display("ERROR: Test %0d failed. Sel=%b, A=%b, B=%b, Y=%b", 
                         i, Sel, A, B, Y);
            else
                $display("PASS: Test %0d", i);
        end
        
        // 测试完成
        $display("All tests completed");
        $finish;
    end
    
    // 波形记录
    initial begin
        $dumpfile("muxN_bin.vcd");
        $dumpvars(0, muxN_bin_tb);
    end
endmodule
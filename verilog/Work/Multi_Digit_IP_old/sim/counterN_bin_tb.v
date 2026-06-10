module counterN_bin_tb;
    parameter N = 8;
    
    reg clk;
    reg rst;
    wire [N-1:0] Q;

    // 实例化被测设计
    counterN_bin #(.N(N)) uut (
        .clk(clk),
        .rst(rst),
        .Q(Q)
    );

    // 时钟生成
    initial begin
        clk = 0;
        forever #5 clk = ~clk;
    end
    
    // 参考计数器
    reg [N-1:0] expected;
    
    // 测试序列
    initial begin
        $display("Time(ns)\tclk\trst\tQ\t\tExpected\tCorrect");
        $display("==================================================================");
        
        // 初始化
        rst = 1;
        expected = 0;
        #12; // 等待足够长时间确保复位生效
        $display("%0t\t%d\t%d\t%b\t%b\t%s", 
                 $time, clk, rst, Q, expected,
                 (Q === expected) ? "right" : "wrong");
        
        // 释放复位
        rst = 0;
        #2; // 短暂等待
        $display("%0t\t%d\t%d\t%b\t%b\t%s", 
                 $time, clk, rst, Q, expected,
                 (Q === expected) ? "right" : "wrong");
        
        // 测试多个时钟周期
        repeat(20) begin
            @(posedge clk); // 等待时钟上升沿
            #1; // 短暂稳定时间
            expected = expected + 1; // 更新期望值
            $display("%0t\t%d\t%d\t%b\t%b\t%s", 
                     $time, clk, rst, Q, expected,
                     (Q === expected) ? "right" : "wrong");
        end
        
        // 测试复位功能
        rst = 1;
        #12;
        expected = 0;
        $display("%0t\t%d\t%d\t%b\t%b\t%s", 
                 $time, clk, rst, Q, expected,
                 (Q === expected) ? "right" : "wrong");
        
        // 再次释放复位
        rst = 0;
        #2;
        $display("%0t\t%d\t%d\t%b\t%b\t%s", 
                 $time, clk, rst, Q, expected,
                 (Q === expected) ? "right" : "wrong");
        
        // 继续测试
        repeat(10) begin
            @(posedge clk); // 等待时钟上升沿
            #1; // 短暂稳定时间
            expected = expected + 1; // 更新期望值
            $display("%0t\t%d\t%d\t%b\t%b\t%s", 
                     $time, clk, rst, Q, expected,
                     (Q === expected) ? "right" : "wrong");
        end
        
        $finish;
    end
endmodule
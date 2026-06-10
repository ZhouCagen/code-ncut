`timescale 1ns / 1ps

module top_module_tb;

    // 输入信号
    reg clk;
    reg rst;
    reg [3:0] N_sel;
    reg [7:0] A_in;
    reg [7:0] B_in;
    reg [3:0] op_sel;
    reg execute;
    
    // 输出信号
    wire [7:0] result;
    wire [2:0] status;
    wire [6:0] seg;
    wire [3:0] an;
    
    // 实例化顶层模块
    top_module uut (
        .clk(clk),
        .rst(rst),
        .N_sel(N_sel),
        .A_in(A_in),
        .B_in(B_in),
        .op_sel(op_sel),
        .execute(execute),
        .result(result),
        .status(status),
        .seg(seg),
        .an(an)
    );
    
    // 时钟生成
    always #5 clk = ~clk;
    
    // 测试任务：执行操作并检查结果
    task test_operation;
        input [3:0] test_op_sel;
        input [3:0] test_N_sel;
        input [7:0] test_A;
        input [7:0] test_B;
        input [15:0] expected_result;
        input [2:0] expected_status;
        begin
            // 设置输入
            op_sel = test_op_sel;
            N_sel = test_N_sel;
            A_in = test_A;
            B_in = test_B;
            
            // 等待一个时钟周期
            @(negedge clk);
            
            // 触发执行
            execute = 1;
            @(negedge clk);
            execute = 0;
            
            // 等待结果稳定
            #20;
            
            // 检查结果
            if (result !== expected_result[7:0] || status !== expected_status) begin
                $display("ERROR: Operation %d, N=%d, A=%h, B=%h", test_op_sel, test_N_sel+1, test_A, test_B);
                $display("  Expected: result=%h, status=%b", expected_result[7:0], expected_status);
                $display("  Got:      result=%h, status=%b", result, status);
            end else begin
                $display("PASS: Operation %d, N=%d, A=%h, B=%h", test_op_sel, test_N_sel+1, test_A, test_B);
                $display("  Result: %h, Status: %b", result, status);
            end
            
            // 等待一段时间
            #50;
        end
    endtask
    
    // 测试过程
    initial begin
        // 初始化
        clk = 0;
        rst = 1;
        N_sel = 0;
        A_in = 0;
        B_in = 0;
        op_sel = 0;
        execute = 0;
        
        // 复位
        #20;
        rst = 0;
        #20;
        
        $display("Starting simulation...");
        
        // 测试加法操作 (op_sel = 1)
        // N=4, A=5, B=3, 预期结果: 5+3=8
        test_operation(4'b0001, 4'b0011, 8'h05, 8'h03, 16'h0008, 3'b000);
        
        // 测试减法操作 (op_sel = 2)
        // N=4, A=8, B=3, 预期结果: 8-3=5
        test_operation(4'b0010, 4'b0011, 8'h08, 8'h03, 16'h0005, 3'b000);
        
        // 测试乘法操作 (op_sel = 3)
        // N=4, A=3, B=4, 预期结果: 3*4=12
        test_operation(4'b0011, 4'b0011, 8'h03, 8'h04, 16'h000C, 3'b000);
        
        // 测试比较操作 (op_sel = 4)
        // N=4, A=5, B=3, 预期状态: A>B (100)
        test_operation(4'b0100, 4'b0011, 8'h05, 8'h03, 16'h0004, 3'b100);
        
        // 测试计数器操作 (op_sel = 5)
        // 这个测试需要多个周期，我们只检查初始值
        test_operation(4'b0101, 4'b0011, 8'h00, 8'h00, 16'h0000, 3'b000);
        
        // 测试奇偶校验操作 (op_sel = 6)
        // N=4, A=5 (二进制0101, 有偶数个1), 预期结果: 0
        test_operation(4'b0110, 4'b0011, 8'h05, 8'h00, 16'h0000, 3'b000);
        
        // 测试多路选择器操作 (op_sel = 7)
        // N=4, A=5, B=3, Sel=B[0]=1, 预期结果: 选择A=5
        test_operation(4'b0111, 4'b0011, 8'h05, 8'h03, 16'h0005, 3'b000);
        
        // 测试译码器操作 (op_sel = 8)
        // N=4, A=3, 预期结果: 第3位为1 (00001000)
        test_operation(4'b1000, 4'b0011, 8'h03, 8'h00, 16'h0008, 3'b000);
        
        // 测试BCD加法操作 (op_sel = 9)
        // N=4, A=5, B=3, 预期结果: 5+3=8
        test_operation(4'b1001, 4'b0011, 8'h05, 8'h03, 16'h0008, 3'b000);
        
        // 测试BCD减法操作 (op_sel = 10)
        // N=4, A=8, B=3, 预期结果: 8-3=5
        test_operation(4'b1010, 4'b0011, 8'h08, 8'h03, 16'h0005, 3'b000);
        
        // 测试不同位宽
        // N=2, A=3, B=2, 加法: 3+2=5 (但N=2, 最大表示3, 所以会溢出)
        test_operation(4'b0001, 4'b0001, 8'h03, 8'h02, 16'h0001, 3'b000); // 3+2=5, 但N=2, 所以结果是1 (5 mod 4)
        
        $display("Simulation completed.");
        $finish;
    end
    
    // 监控输出
    initial begin
        $monitor("Time=%t, op_sel=%d, N_sel=%d, A=%h, B=%h, result=%h, status=%b", 
                 $time, op_sel, N_sel, A_in, B_in, result, status);
    end
    
    // 设置仿真时长
    initial begin
        #2000;
        $display("Simulation timeout.");
        $finish;
    end

endmodule
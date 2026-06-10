`timescale 1ns/1ps

module pipeline_cpu_tb;

    // 输入信号
    reg clk;
    reg rst_n;
    
    // 实例化CPU
    pipeline_cpu uut (
        .clk(clk),
        .rst_n(rst_n)
    );
    
    // 时钟生成
    always #5 clk = ~clk;
    
    // 测试序列
    initial begin
        // 初始化信号
        clk = 0;
        rst_n = 0;
        
        // 生成VCD波形文件
        $dumpfile("pipeline_cpu.vcd");
        $dumpvars(0, pipeline_cpu_tb);
        
        // 复位系统
        #10 rst_n = 1;
        
        // 运行足够长时间以完成测试程序
        #1000;
        
        // 结束仿真
        $display("Simulation completed");
        $finish;
    end
    
    // 监控关键信号
    always @(posedge clk) begin
        $display("Time: %0t, PC: %h", $time, uut.pc_current);
        
        // 监控冒险检测信号
        $display("  Hazard: pc_write_en=%b, if_id_write_en=%b, hazard_stall=%b", 
                 uut.pc_write_en, uut.if_id_write_en, uut.hazard_stall);
        $display("  EX_MEM: mem_read=%b, rd=%h", uut.ex_mem_read, uut.ex_rd);
        
        // 监控前递信号
        $display("  Forward: A=%b, B=%b", uut.forward_a, uut.forward_b);
        
        // 当PC进入循环时显示更多信息
        if (uut.pc_current == 32'h48 || uut.pc_current == 32'h4c || uut.pc_current == 32'h50) begin
            $display("  LOOP DETECTED! Instruction at PC %h: %h", 
                     uut.pc_current, uut.instruction_if);
            $display("  ID Stage: Instr=%h, RS=%0d, RT=%0d", 
                     uut.instruction_id, uut.instruction_id[25:21], uut.instruction_id[20:16]);
        end
    end

endmodule
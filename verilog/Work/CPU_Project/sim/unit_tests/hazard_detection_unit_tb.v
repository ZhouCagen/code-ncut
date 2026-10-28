`timescale 1ns/1ps

module hazard_detection_unit_tb;
    // 输入信号
    reg [4:0] id_rs;
    reg [4:0] id_rt;
    reg [4:0] ex_rd;
    reg ex_mem_read;
    
    // 输出信号
    wire pc_write_en;
    wire if_id_write_en;
    wire control_mux_sel;
    
    // 实例化被测试的冒险检测单元
    hazard_detection_unit dut (
        .id_rs(id_rs),
        .id_rt(id_rt),
        .ex_rd(ex_rd),
        .ex_mem_read(ex_mem_read),
        .pc_write_en(pc_write_en),
        .if_id_write_en(if_id_write_en),
        .control_mux_sel(control_mux_sel)
    );
    
    // 测试过程
    initial begin
        // 初始化输入信号
        id_rs = 5'd0;
        id_rt = 5'd0;
        ex_rd = 5'd0;
        ex_mem_read = 1'b0;
        
        // 监控信号变化
        $monitor("Time=%0t: id_rs=%0d, id_rt=%0d, ex_rd=%0d, ex_mem_read=%b, pc_write_en=%b, if_id_write_en=%b, control_mux_sel=%b", 
                 $time, id_rs, id_rt, ex_rd, ex_mem_read, pc_write_en, if_id_write_en, control_mux_sel);
        
        // 测试用例1: 无冒险情况
        #10;
        id_rs = 5'd1;
        id_rt = 5'd2;
        ex_rd = 5'd3;
        ex_mem_read = 1'b0; // 不是load指令
        #10;
        $display("Test 1 - No hazard: All enables should be 1, mux_sel=0");
        
        // 测试用例2: 是load指令但寄存器不匹配
        #10;
        id_rs = 5'd1;
        id_rt = 5'd2;
        ex_rd = 5'd4;
        ex_mem_read = 1'b1; // 是load指令
        #10;
        $display("Test 2 - Load instruction but no register match: All enables should be 1, mux_sel=0");
        
        // 测试用例3: Load-Use冒险 - rs匹配
        #10;
        id_rs = 5'd5; // 与ex_rd相同
        id_rt = 5'd2;
        ex_rd = 5'd5;
        ex_mem_read = 1'b1; // 是load指令
        #10;
        $display("Test 3 - Load-Use hazard (rs match): pc_write_en=0, if_id_write_en=0, control_mux_sel=1");
        
        // 测试用例4: Load-Use冒险 - rt匹配
        #10;
        id_rs = 5'd1;
        id_rt = 5'd6; // 与ex_rd相同
        ex_rd = 5'd6;
        ex_mem_read = 1'b1; // 是load指令
        #10;
        $display("Test 4 - Load-Use hazard (rt match): pc_write_en=0, if_id_write_en=0, control_mux_sel=1");
        
        // 测试用例5: Load-Use冒险 - rs和rt都匹配
        #10;
        id_rs = 5'd7; // 与ex_rd相同
        id_rt = 5'd7; // 与ex_rd相同
        ex_rd = 5'd7;
        ex_mem_read = 1'b1; // 是load指令
        #10;
        $display("Test 5 - Load-Use hazard (both rs and rt match): pc_write_en=0, if_id_write_en=0, control_mux_sel=1");
        
        // 测试用例6: 寄存器0的特殊情况
        #10;
        id_rs = 5'd0; // 寄存器0
        id_rt = 5'd2;
        ex_rd = 5'd0; // 寄存器0
        ex_mem_read = 1'b1; // 是load指令
        #10;
        $display("Test 6 - Register 0 special case: All enables should be 1, mux_sel=0 (no hazard for $0)");
        
        // 测试用例7: 非load指令但寄存器匹配
        #10;
        id_rs = 5'd8; // 与ex_rd相同
        id_rt = 5'd2;
        ex_rd = 5'd8;
        ex_mem_read = 1'b0; // 不是load指令
        #10;
        $display("Test 7 - Non-load instruction with register match: All enables should be 1, mux_sel=0");
        
        // 结束仿真
        #10;
        $display("Simulation finished.");
        $finish;
    end
endmodule
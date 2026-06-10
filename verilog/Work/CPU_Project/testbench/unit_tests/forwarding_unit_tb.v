`timescale 1ns/1ps

module forwarding_unit_tb;
    // 输入信号
    reg [4:0] ex_rs;
    reg [4:0] ex_rt;
    reg [4:0] mem_rd;
    reg mem_reg_write;
    reg [4:0] wb_rd;
    reg wb_reg_write;
    
    // 输出信号
    wire [1:0] forward_a;
    wire [1:0] forward_b;
    
    // 实例化被测试的前递单元
    forwarding_unit dut (
        .ex_rs(ex_rs),
        .ex_rt(ex_rt),
        .mem_rd(mem_rd),
        .mem_reg_write(mem_reg_write),
        .wb_rd(wb_rd),
        .wb_reg_write(wb_reg_write),
        .forward_a(forward_a),
        .forward_b(forward_b)
    );
    
    // 测试过程
    initial begin
        // 初始化输入信号
        ex_rs = 5'd0;
        ex_rt = 5'd0;
        mem_rd = 5'd0;
        mem_reg_write = 1'b0;
        wb_rd = 5'd0;
        wb_reg_write = 1'b0;
        
        // 测试用例1: 无数据冒险
        #10;
        ex_rs = 5'd1;
        ex_rt = 5'd2;
        mem_rd = 5'd3;
        mem_reg_write = 1'b1;
        wb_rd = 5'd4;
        wb_reg_write = 1'b1;
        #10;
        $display("Test 1 - No hazard: forward_a=%b, forward_b=%b (Expected: 00, 00)", forward_a, forward_b);
        
        // 测试用例2: MEM阶段前递到rs
        #10;
        ex_rs = 5'd5;
        ex_rt = 5'd2;
        mem_rd = 5'd5;  // 与ex_rs相同
        mem_reg_write = 1'b1;
        wb_rd = 5'd4;
        wb_reg_write = 1'b1;
        #10;
        $display("Test 2 - MEM forward to rs: forward_a=%b, forward_b=%b (Expected: 10, 00)", forward_a, forward_b);
        
        // 测试用例3: MEM阶段前递到rt
        #10;
        ex_rs = 5'd1;
        ex_rt = 5'd6;  
        mem_rd = 5'd6;  // 与ex_rt相同
        mem_reg_write = 1'b1;
        wb_rd = 5'd4;
        wb_reg_write = 1'b1;
        #10;
        $display("Test 3 - MEM forward to rt: forward_a=%b, forward_b=%b (Expected: 00, 10)", forward_a, forward_b);
        
        // 测试用例4: WB阶段前递到rs
        #10;
        ex_rs = 5'd7;
        ex_rt = 5'd2;
        mem_rd = 5'd8;
        mem_reg_write = 1'b0;  // MEM不写寄存器
        wb_rd = 5'd7;  // 与ex_rs相同
        wb_reg_write = 1'b1;
        #10;
        $display("Test 4 - WB forward to rs: forward_a=%b, forward_b=%b (Expected: 01, 00)", forward_a, forward_b);
        
        // 测试用例5: WB阶段前递到rt
        #10;
        ex_rs = 5'd1;
        ex_rt = 5'd9;
        mem_rd = 5'd8;
        mem_reg_write = 1'b0;  // MEM不写寄存器
        wb_rd = 5'd9;  // 与ex_rt相同
        wb_reg_write = 1'b1;
        #10;
        $display("Test 5 - WB forward to rt: forward_a=%b, forward_b=%b (Expected: 00, 01)", forward_a, forward_b);
        
        // 测试用例6: MEM和WB同时可前递到rs (MEM优先级更高)
        #10;
        ex_rs = 5'd10;
        ex_rt = 5'd2;
        mem_rd = 5'd10;  // 与ex_rs相同
        mem_reg_write = 1'b1;
        wb_rd = 5'd10;  // 也与ex_rs相同
        wb_reg_write = 1'b1;
        #10;
        $display("Test 6 - MEM and WB both forward to rs: forward_a=%b, forward_b=%b (Expected: 10, 00)", forward_a, forward_b);
        
        // 测试用例7: MEM前递到rs，WB前递到rt
        #10;
        ex_rs = 5'd11;
        ex_rt = 5'd12;
        mem_rd = 5'd11;  // 与ex_rs相同
        mem_reg_write = 1'b1;
        wb_rd = 5'd12;  // 与ex_rt相同
        wb_reg_write = 1'b1;
        #10;
        $display("Test 7 - MEM forward to rs, WB forward to rt: forward_a=%b, forward_b=%b (Expected: 10, 01)", forward_a, forward_b);
        
        // 测试用例8: 寄存器0的特殊情况 (不应前递)
        #10;
        ex_rs = 5'd0;
        ex_rt = 5'd2;
        mem_rd = 5'd0;  // 寄存器0
        mem_reg_write = 1'b1;
        wb_rd = 5'd4;
        wb_reg_write = 1'b1;
        #10;
        $display("Test 8 - Register 0 special case: forward_a=%b, forward_b=%b (Expected: 00, 00)", forward_a, forward_b);
        
        // 测试用例9: MEM阶段不写寄存器
        #10;
        ex_rs = 5'd13;
        ex_rt = 5'd2;
        mem_rd = 5'd13;  // 与ex_rs相同
        mem_reg_write = 1'b0;  // 关键: MEM不写寄存器
        wb_rd = 5'd4;
        wb_reg_write = 1'b1;
        #10;
        $display("Test 9 - MEM not writing: forward_a=%b, forward_b=%b (Expected: 00, 00)", forward_a, forward_b);
        
        // 结束仿真
        #10;
        $display("Simulation finished.");
        $finish;
    end
endmodule
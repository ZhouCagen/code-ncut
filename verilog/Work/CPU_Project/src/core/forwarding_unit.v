`timescale 1ns/1ps

module forwarding_unit(
    input wire [4:0] ex_rs,     // EX阶段（执行阶段）的源寄存器rs
    input wire [4:0] ex_rt,     // EX阶段（执行阶段）的源寄存器rt
    input wire [4:0] mem_rd,    // MEM阶段（内存访问阶段）的目标寄存器rd
    input wire mem_reg_write,   // MEM阶段（内存访问阶段）是否写寄存器
    input wire [4:0] wb_rd,     // WB阶段（写回阶段）的目标寄存器
    input wire wb_reg_write,    // WB阶段（写回阶段）是否写寄存器

    output reg [1:0] forward_a,    // 前递选择信号a（用于rs）
    output reg [1:0] forward_b     // 前递选择信号b（用于rt）
);

    always @(*) begin
        
        forward_a = 2'b00;
        forward_b = 2'b00;

        // 前递rs寄存器
        // 情况1：EX/MEM前递（优先级最高）- 前递MEM阶段的结果
        if(mem_reg_write && (mem_rd != 0) && (mem_rd == ex_rs)) begin
            forward_a = 2'b10;  // 选择MEM阶段的结果
        // 情况2：MEM/WB前递 - 前递WB阶段的结果             
        end else if(wb_reg_write && (wb_rd != 0) && (wb_rd == ex_rs)) begin
            forward_a = 2'b01;  // 选择WB阶段的结果
        end

        // 前递rt寄存器
        // 情况1：EX/MEM前递（优先级最高）- 前递MEM阶段的结果
        if (mem_reg_write && (mem_rd != 0) && (mem_rd == ex_rt)) begin
            forward_b = 2'b10;  // 选择MEM阶段的结果
        end
        // 情况2：MEM/WB前递 - 前递WB阶段的结果
        else if (wb_reg_write && (wb_rd != 0) && (wb_rd == ex_rt)) begin
            forward_b = 2'b01;  // 选择WB阶段的结果
        end
        //  EX/MEM 前递 eg：
        //      sub $t0, $t1, $t2   # 指令SUB: $t0 = $t1 - $t2      时钟周期跟踪（关键周期）：
        //      add $s0, $t0, $t3   # 指令ADD: $s0 = $t0 + $t3      |时间| IF | ID | EX | MEM | WB | 
        //      $t0还在MEM阶段 但add已经需要$t0进行EX阶段              Cycle4          add  sub      

        //  EX/WB  前递 eg：
        //      sub $t0, $t1, $t2   # 指令1 (SUB)                               | 时间 | IF | ID | EX | MEM | WB |
        //      add $t4, $t5, $t6   # 指令2 (ADD) - 这条指令隔开了SUB和AND         Cycle5           and  add   sub
        //      and $s0, $t0, $t3   # 指令3 (AND): 需要SUB的结果$t0     
        //  sub指令在WB阶段。它的结果($t1-$t2)已经通过了MEM阶段，现在安稳地放在MEM/WB.WriteData寄存器里，正准备在本周期末写回$t0
        //  and指令在EX阶段。它需要$t0的值。从寄存器文件读出的$t0仍然是旧值（因为sub的写回尚未完成）。
    end

endmodule
`timescale 1ns/1ps

// 冒险检测单元：主要任务是检测那些无法通过前递（Forwarding）解决的数据冒险，并采取相应措施。最主要的就是 Load-Use Hazard。
// lw $t0, 0($s0)    # 从内存加载数据到$t0 (LOAD指令)       lw 在 MEM 阶段结束 才从内存中取得数据寄存在 EX/MEM 寄存器
// add $s1, $t0, $s2 # 立即使用$t0                         add 在 EX 阶段开始时就需要操作数 但此时 $t0 的数据还在内存中，尚未读取完成
// 唯一的解决方案是让 add 指令在 ID 阶段多停留一个周期：
// 时钟周期	IF	ID    EX	  MEM	  WB	   说明
// 3	       add	 lw	         	        检测到冒险
// 4	       add	bubble	  lw		     插入停顿
// 5	             add	bubble	  lw   数据就绪，可前递      lw 进入 WB阶段：数据已经在 MEM/WB 寄存器中可用 add 进入 EX阶段：此时可以通过 MEM/WB 前递获得 $t0 的值

module hazard_detection_unit(
    input wire [4:0] id_rs,
    input wire [4:0] id_rt,
    input wire [4:0] ex_rd,
    input wire ex_mem_read,         // EX阶段是否是load指令（内存读取）
    output reg pc_write_en,         // PC写使能（1=允许更新，0=暂停）
    output reg if_id_write_en,      // IF/ID寄存器写使能（1=允许更新，0=暂停）
    output reg control_mux_sel      // 控制信号选择（1=插入空操作，0=正常） 
);

    always @(*) begin
        // 默认值：无冒险，允许流水线正常流动
        pc_write_en = 1;
        if_id_write_en = 1;
        control_mux_sel = 0;

        // 检测 Load-Use 冒险
        if(ex_mem_read && (ex_rd != 0) && ((ex_rd == id_rs)||(ex_rd == id_rt))) begin
            pc_write_en = 1'b0;
            if_id_write_en = 1'b0;
            control_mux_sel = 1'b1;
        end
    end
endmodule

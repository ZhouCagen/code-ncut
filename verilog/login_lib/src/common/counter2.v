module counter2(
    input wire clk,  // 时钟输入。计数器按时钟上升沿递增。
    input wire rst,  // 异步复位信号，高有效。异步：不依赖时钟，rst=1 时立即把计数器清零。
    output reg [1:0] Q // 2位计数器输出
);

    always @(posedge clk or posedge rst) begin 
    // 触发条件：posedge clk → 时钟上升沿触发 posedge rst → 复位上升沿触发
        if (rst) //当复位信号为高时执行 Q <= 2'b00; → 异步复位计数器清零
            Q <= 2'b00;
        else //如果复位不生效，每个时钟上升沿计数器 +1 Q 的二进制加法自然产生翻转和进位
             //Q 循环：00 → 01 → 10 → 11 → 00 ...
            Q <= Q + 1'b1;
     end       
     // <= 非阻塞赋值，适合同步逻辑，保证多个寄存器同时更新而不互相干扰
endmodule
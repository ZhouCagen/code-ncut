module counter2_bin(
    input wire clk,
    input wire rst,
    input wire en,  // 使能信号
    output reg [1:0] Q,
    output wire carry_out  // 进位输出
);

    always @(posedge clk or posedge rst) begin
        if(rst)
            Q <= 2'b00;
        else if(en)
            Q <= Q + 1;
    end

    // 进位输出：当计数器达到最大值时产生进位
    assign carry_out = (Q == 2'b11) & en;
endmodule
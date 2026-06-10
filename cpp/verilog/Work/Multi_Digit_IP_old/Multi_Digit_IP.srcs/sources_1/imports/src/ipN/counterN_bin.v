module counterN_bin #(parameter N = 8)(
    input wire clk,
    input wire rst,
    output wire [N-1:0] Q
);

    // 计算需要多少个2位计数器
    localparam Num_Blocks = (N + 1) / 2;
    
    // 进位信号
    wire [Num_Blocks:0] carry;
    assign carry[0] = 1'b1;  // 最低位始终使能
    
    // 临时输出
    wire [2*Num_Blocks-1:0] Q_temp;
    
    genvar i;
    generate 
        for(i = 0; i < Num_Blocks; i = i + 1) begin
            counter2_bin u_counter2_bin(
                .clk(clk),
                .rst(rst),
                .en(carry[i]),
                .Q(Q_temp[2*i +: 2]),
                .carry_out(carry[i+1])
            );
        end
    endgenerate
    
    // 提取有效位
    assign Q = Q_temp[N-1:0];

endmodule
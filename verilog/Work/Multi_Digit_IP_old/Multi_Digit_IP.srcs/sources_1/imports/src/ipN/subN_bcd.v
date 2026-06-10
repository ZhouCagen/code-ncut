module subN_bcd #(parameter N = 8)(
    input wire [N-1:0] A,
    input wire [N-1:0] B,
    input wire Bin,
    output wire [N-1:0] Diff,
    output wire Bout
);
    localparam integer Digits = (N + 3) / 4;  // 计算BCD位数
    localparam integer PAD = 4 * Digits - N;   // 需要补齐的位数

    wire [4*Digits-1:0] A_pad = {{PAD{1'b0}}, A};  // 用零补齐A
    wire [4*Digits-1:0] B_pad = {{PAD{1'b0}}, B};  // 用零补齐B

    wire [Digits:0] borrow;  // 借位信号
    assign borrow[0] = Bin;

    wire [4*Digits-1:0] Diff_pad;  // 补齐后的减法结果

    genvar i;
    generate
        for(i = 0; i < Digits; i = i + 1) begin
            sub2_bcd u_sub2_bcd(
                .A(A_pad[4*i +: 4]),
                .B(B_pad[4*i +: 4]),
                .Bin(borrow[i]),
                .Diff(Diff_pad[4*i +: 4]),
                .Bout(borrow[i+1])
            );
        end
    endgenerate

    assign Diff = Diff_pad[N-1:0];   // 提取相关位
    assign Bout = borrow[Digits];    // 最终借位输出
    
endmodule
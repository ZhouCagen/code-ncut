module subN_bin #(parameter N = 8)(
    input wire [N-1:0] A,
    input wire [N-1:0] B,
    input wire Bin,
    output wire [N-1:0] Diff,
    output wire Bout
);
    localparam integer Pairs = (N + 1) / 2;  // 计算需要多少个2位减法器
    localparam integer PAD = 2 * Pairs - N;   // 需要补齐的位数

    wire [2*Pairs-1:0] A_pad = {{PAD{1'b0}}, A};  // 用零补齐A
    wire [2*Pairs-1:0] B_pad = {{PAD{1'b0}}, B};  // 用零补齐B

    wire [Pairs:0] borrow;  // 借位信号
    assign borrow[0] = Bin;

    wire [2*Pairs-1:0] Diff_pad;  // 补齐后的减法结果

    genvar i;
    generate
        for(i = 0; i < Pairs; i = i + 1) begin
            sub2_bin u_sub2_bin(
                .A(A_pad[2*i +: 2]),
                .B(B_pad[2*i +: 2]),
                .Bin(borrow[i]),
                .Diff(Diff_pad[2*i +: 2]),
                .Bout(borrow[i+1])
            );
        end
    endgenerate

    assign Diff = Diff_pad[N-1:0];   // 提取相关位
    assign Bout = borrow[Pairs];     // 最终借位输出
endmodule
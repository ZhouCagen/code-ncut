module cmpN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    output wire A_gt_B,
    output wire A_eq_B,
    output wire A_lt_B
);

    // 确保N是偶数
    localparam HALF_N = N/2;
    
    wire [HALF_N-1:0] gt, eq, lt;
    genvar i;
    
    generate 
        for(i = 0; i < HALF_N; i = i + 1) begin
            cmp2_bin u_cmp2_bin(
                .A(A[2*i+1:2*i]),
                .B(B[2*i+1:2*i]),
                .A_gt_B(gt[i]),
                .A_eq_B(eq[i]),
                .A_lt_B(lt[i])
            );
        end
    endgenerate

    // 从最高位开始比较
    integer j;
    reg gt_flag, eq_flag, lt_flag;
    reg found;

    always @(*) begin
        gt_flag = 0;
        eq_flag = 1;
        lt_flag = 0;
        found = 0;

        for(j = HALF_N-1; j >= 0; j = j - 1) begin
            if (!found && !eq[j]) begin
                // 找到第一个不相等的位对
                gt_flag = gt[j];
                eq_flag = 0;
                lt_flag = lt[j];
                found = 1;
            end
        end
    end

    assign A_gt_B = gt_flag;
    assign A_eq_B = eq_flag;
    assign A_lt_B = lt_flag;

endmodule
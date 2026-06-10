module cmpN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    output A_gt_B,
    output A_eq_B,
    output A_lt_B
);

    wire [N/2-1:0] gt, eq ,lt;
    genvar i;
    
    generate 
        for(i = 0; i < N/2; i = i + 1) begin
            cmp2_bin u_cmp2_bin(
                .A(A[N-2*i-1 -:2]),
                .B(B[N-2*i-1 -:2]),
                .A_gt_B(gt[i]),
                .A_eq_B(eq[i]),
                .A_lt_B(lt[i])
            );
        end
    endgenerate

    integer j;
    reg gt_flag, eq_flag, lt_flag;

    always @(*) begin : for_loop
        gt_flag = 0;
        eq_flag = 1;
        lt_flag = 0;

        for(j = 0; j < N/2; j = j + 1) begin
            if(eq[j]) begin
                continue;
            end else if(gt[j]) begin
                gt_flag = 1;
                eq_flag = 0;
                lt_flag = 0;
                disable for_loop;
            end else if(lt[j]) begin
                gt_flag = 0;
                eq_flag = 0;
                lt_flag = 1;
                disable for_loop;
            end
        end
    end

    assign A_gt_B = gt_flag;
    assign A_eq_B = eq_flag;
    assign A_lt_B = lt_flag;

endmodule        
module cmp2_bin(
    input wire [1:0] A, B,
    output wire A_gt_B, A_eq_B, A_lt_B
);

    assign A_gt_B = (A > B);
    assign A_eq_B = (A == B);
    assign A_lt_B = (A < B);    

endmodule
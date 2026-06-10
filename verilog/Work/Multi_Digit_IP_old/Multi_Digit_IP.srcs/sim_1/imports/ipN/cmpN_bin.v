module cmpN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    output wire A_gt_B,
    output wire A_eq_B,
    output wire A_lt_B
);

    assign A_gt_B = A > B;
    assign A_eq_B = A == B;
    assign A_lt_B = A < B;

endmodule    
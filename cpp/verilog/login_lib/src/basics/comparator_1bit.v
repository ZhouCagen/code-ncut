module comparator_1bit(
    input wire A,
    input wire B,
    input wire Cin_gt,  // 来自低位，A已大于B
    input wire Cin_eq,  // 来自低位，A等于B
    input wire Cin_lt,  // 来自低位，A已小于B
    output wire A_gt_B,
    output wire A_eq_B,
    output wire A_lt_B
);
    assign A_gt_B = (Cin_eq & (A & ~B)) | Cin_gt;
    assign A_eq_B = (Cin_eq & ~(A ^ B));
    assign A_lt_B = (Cin_eq & (~A & B)) | Cin_lt;

endmodule
module comp4(
    input wire [3:0] A,
    input wire [3:0] B,
    output wire A_gt_B,
    output wire A_eq_B,
    output wire A_lt_B
);

    wire gt3, eq3, lt3;
    wire gt2, eq2, lt2;
    wire gt1, eq1, lt1;
    
    comparator_1bit c3(.A(A[3]), .B(B[3]), .Cin_gt(0), .Cin_eq(1), .Cin_lt(0),
                       .A_gt_B(gt3), .A_eq_B(eq3), .A_lt_B(lt3));
    comparator_1bit c2(.A(A[2]), .B(B[2]), .Cin_gt(gt3), .Cin_eq(eq3), .Cin_lt(lt3),
                       .A_gt_B(gt2), .A_eq_B(eq2), .A_lt_B(lt2));
    comparator_1bit c1(.A(A[1]), .B(B[1]), .Cin_gt(gt2), .Cin_eq(eq2), .Cin_lt(lt2),
                       .A_gt_B(gt1), .A_eq_B(eq1), .A_lt_B(lt1));
    comparator_1bit c0(.A(A[0]), .B(B[0]), .Cin_gt(gt1), .Cin_eq(eq1), .Cin_lt(lt1),
                       .A_gt_B(A_gt_B), .A_eq_B(A_eq_B), .A_lt_B(A_lt_B));  
                                    
endmodule
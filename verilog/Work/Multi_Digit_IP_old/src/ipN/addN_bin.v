module addN_bin #(parameter N = 8)(
    input wire [N-1:0] A, B,
    input wire Cin,
    output wire [N-1:0] Sum,
    output wire Cout
);
    
    localparam M = (N + 1)/ 2; 
    wire [M:0] carry;
    assign carry[0] = Cin;
    
    genvar i;
    generate 
        for(i = 0 ; i< M; i= i + 1) begin
            if (i*2 +1 < N) begin
                add2_bin u_add2_bin (
                    .A(A[i*2 +: 2]),
                    .B(B[i*2 +: 2]),
                    .Cin(carry[i]),
                    .Sum(Sum[i*2 +: 2]),
                    .Cout(carry[i+1])
                );
             end else begin
                wire [1:0] a_last = {1'b0, A[i*2]};
                wire [1:0] b_last = {1'b0, B[i*2]};
                wire [1:0] sum_last;
                
                add2_bin u_add2_bin_last (
                    .A(a_last),
                    .B(b_last),
                    .Cin(carry[i]),
                    .Sum(sum_last),
                    .Cout(carry[i+1])
                );   
                
                assign Sum[i*2] = sum_last[0];
                
            end    
        end 
    endgenerate         
    
    assign Cout = carry[M];
    
endmodule
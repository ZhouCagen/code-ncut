module subN_bcd #(parameter N = 8)(
    input wire [4*N-1:0] A, B,
    input wire Bin,
    output reg [4*N-1:0] Diff,
    output reg Bout
);
    
    reg [3:0] a_digit [0:N-1];
    reg [3:0] b_digit [0:N-1];
    reg [3:0] diff_digit [0:N-1];
    reg borrow;
    
    integer i;
    
    always @(*) begin
        for(i = 0; i < N; i = i + 1) begin
            a_digit[i] = A[4*(N-1-i) +: 4];
            b_digit[i] = B[4*(N-1-i) +: 4];   
        end   
        
        borrow = Bin;
        for(i = N - 1; i >= 0; i = i - 1)begin
            if(a_digit[i] - b_digit[i] - borrow < 0) begin
                diff_digit[i] = a_digit[i] - b_digit[i] - borrow +10;
                borrow = 1;
            end else begin
                diff_digit[i] = a_digit[i] - b_digit[i] - borrow;
                borrow = 0;
            end   
        end    
        
        for(i = 0; i < N; i = i + 1) begin
            Diff[4*(N-1-i) +:4] = diff_digit[i];
        end
        
        Bout = borrow;
    end

endmodule
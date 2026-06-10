module addN_bin #(parameter N = 8)(
    input wire [N-1:0] A,
    input wire [N-1:0] B,
    input wire Cin,
    output wire [N-1:0] Sum,
    output wire Cout
);
    localparam integer Pairs = (N + 1) / 2;  
    localparam integer PAD = 2 * Pairs - N;   

    wire [2*Pairs-1:0] A_pad = {{PAD{1'b0}}, A};  
    wire [2*Pairs-1:0] B_pad = {{PAD{1'b0}}, B};  

    wire [Pairs:0] carry;  
    assign carry[0] = Cin;

    wire [2*Pairs-1:0] Sum_pad;  

    genvar i;
    generate
        for(i = 0; i < Pairs; i = i + 1) begin
            add2_bin u_add2_bin(
                .A(A_pad[2*i +: 2]),
                .B(B_pad[2*i +: 2]),
                .Cin(carry[i]),
                .Sum(Sum_pad[2*i +: 2]),
                .Cout(carry[i+1])
            );
        end
    endgenerate

    assign Sum = Sum_pad[N-1:0];  
    assign Cout = carry[Pairs];    
endmodule
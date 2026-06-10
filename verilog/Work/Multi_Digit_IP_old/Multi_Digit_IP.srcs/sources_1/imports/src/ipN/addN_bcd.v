module addN_bcd #(parameter N = 8)(
    input wire [N-1:0] A, B,
    input wire Cin,
    output wire [N-1:0] Sum,
    output wire Cout
);
    localparam integer Digits = (N + 3) / 4;  // Calculate number of BCD digits
    localparam integer PAD = 4 * Digits - N;   // Padding bits needed

    wire [4*Digits-1:0] A_pad = {{PAD{1'b0}}, A};  // Pad A with zeros
    wire [4*Digits-1:0] B_pad = {{PAD{1'b0}}, B};  // Pad B with zeros

    wire [Digits:0] carry;  // Carry signals between digits
    assign carry[0] = Cin;

    wire [4*Digits-1:0] Sum_pad;  // Padded sum result

    genvar i;
    generate
        for(i = 0; i < Digits; i = i + 1) begin
            add2_bcd u_add2_bcd(
                .A(A_pad[4*i +: 4]),  // Slice of A
                .B(B_pad[4*i +: 4]),  // Slice of B
                .Cin(carry[i]),       // Carry in
                .Sum(Sum_pad[4*i +: 4]),
                .Cout(carry[i+1])     // Carry out to next digit
            );
        end
    endgenerate

    assign Sum = Sum_pad[N-1:0];   // Extract relevant bits
    assign Cout = carry[Digits];    // Final carry out
endmodule
module add2_bcd(
    input wire [3:0] A, B,
    input wire Cin,
    output wire [3:0] Sum,
    output wire Cout
);

    wire [4:0] temp;
    assign temp = A + B + Cin;
    
    // BCD 校正：如果加法结果大于 9，则减去 10
    assign Sum  = (temp > 9) ? (temp - 10) : temp[3:0];
    
    // 如果加法结果大于 9，则产生进位
    assign Cout = (temp > 9) ? 1'b1 : 1'b0;

endmodule

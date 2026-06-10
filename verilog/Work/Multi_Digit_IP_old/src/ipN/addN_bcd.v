module addN_bcd #(parameter N = 8)(
    input wire [N-1:0] A, B,
    input wire Cin,
    output reg [N-1:0] Sum,
    output reg Cout
);
    // 计算需要的 digit 数（向上取整）和需要补的 PAD 位数（MSB 侧）
    localparam integer Digits = (N + 3) / 4;
    localparam integer PAD = 4 * Digits - N;
    
    // {PAD{1'b0}} 是 Verilog 的重复连接运算符
    wire [4*Digits-1:0] A_pad = {{PAD{1'b0}}, A};
    wire [4*Digits-1:0] B_pad = {{PAD{1'b0}}, B};
    
    wire [Digits:0] carry;
    assign carry[0] = Cin;
    
    wire [4*Digits-1:0] Sum_pad;
    
    genvar i;
    generate 
        for(i = 0; i< N; i= i + 1) begin
            add2_bcd u_add2_bcd(
            .A(A_pad[4*i +: 4]),    // 从低位开始切片
            .B(B_pad[4*i +: 4]),
            .Cin(carry[i]),
            .Sum(Sum_pad[4*i +: 4]),
            .Cout(carry[i+1])
            
            );
        end
    endgenerate
    
    always @* begin
        Sum  = Sum_pad[N-1:0];    // 截取原始宽度
        Cout = carry[Digits];
    end
    
endmodule
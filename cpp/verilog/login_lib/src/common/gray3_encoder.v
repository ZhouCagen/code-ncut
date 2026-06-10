// Gray 编码公式（n 位）：
// 对于 n 位二进制输入 B[n-1:0]，输出 Gray 编码 G[n-1:0]：
// G[n-1]=B[n-1]（最高位不变） G[i]=B[i+1]⊕B[i],i=n-2,...,0

module gray3_encoder(
    input wire [2:0] bin,
    output wire [2:0] gray
);
    assign gray[2] = bin[2];
    assign gray[1] = bin[2] ^ bin[1];
    assign gray[0] = bin[1] ^ bin[0];
   
endmodule
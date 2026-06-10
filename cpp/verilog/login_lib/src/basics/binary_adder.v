module binary_adder #(parameter N=8)(
    input [N-1:0] A,B,
    input Cin,
    output [N-1:0] Sum,
    output Cout
);

    assign {Cout,Sum} = A + B + Cin;
// 在多位二进制加法器里，每一位的 Cin 都是它低一位的 Cout。
// 唯一例外是最低位的 Cin，通常人为设为 0（因为最低位没有低位进位）
// {Cout, Sum} = A + B + Cin 的原理 A + B + Cin Verilog 支持直接对向量（多位数）做加法
//假设 A 和 B 是 N 位，Cin 是 1 位相加后的结果可能是 N+1 位（因为可能溢出），高位就是进位 Cout，低 N 位就是和 Sum
endmodule
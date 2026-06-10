module decimal_adder(
    input [3:0] A,
    input [3:0] B,
    input Cin,
    output [3:0] Sum,
    output Cout
);

    wire [4:0] temp;
    assign temp = A + B + Cin;
// BCD修正
    assign {Cout,Sum} = (temp > 9) ? (temp + 6) : temp;
//原理：为什么要加 6？BCD 每位只能表示 0~9如果 A+B+Cin > 9（例如 7+5=12），二进制结果 temp = 12 = 01100
//     BCD 规则：当二进制结果大于 9 时，加 6 可以把它转换成正确的十进制表示 
//    例：12 + 6 = 18 = 10010 低 4 位 Sum = 0010 (2) 高位 Cout = 1 → 表示向高位进 1 {Cout, Sum} 拼接
//    花括号 {} 是拼接操作 高位 Cout 表示是否进位 低 4 位 Sum 是修正后的 BCD 值
    
endmodule
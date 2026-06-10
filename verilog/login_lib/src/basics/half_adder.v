module half_adder(
    input  A,
    input  B,
    output Sum,
    output Carry
);
    assign Sum   = A ^ B;  // 异或得到和
    assign Carry = A & B;  // 与得到进位
endmodule

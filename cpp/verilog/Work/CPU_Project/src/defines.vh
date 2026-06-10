// ===========指令Opcode定义===========
`define OP_RTYPE 6'b00_0000   // R-Type指令的操作码
`define OP_ADDI  6'b00_1000
`define OP_ADDIU 6'b00_1001
`define OP_ANDI  6'b00_1100
`define OP_ORI   6'b00_1101
`define OP_XORI  6'b00_1110
`define OP_LUI   6'b00_1111
`define OP_LW    6'b10_0011
`define OP_SW    6'b10_1011
`define OP_BEQ   6'b00_0100
`define OP_BNE   6'b00_0101
`define OP_SLTI  6'b00_1010
`define OP_SLTIU 6'b00_1011
`define OP_J     6'b00_0010
`define OP_JAL   6'b00_0011

// =======R-Type指令的Funct字段定义=======

`define FUNCT_ADD  6'b10_0000
`define FUNCT_ADDU 6'b10_0001
`define FUNCT_SUB  6'b10_0010
`define FUNCT_SUBU 6'b10_0011
`define FUNCT_AND  6'b10_0100
`define FUNCT_OR   6'b10_0101
`define FUNCT_XOR  6'b10_0110
`define FUNCT_NOR  6'b10_0111
`define FUNCT_SLT  6'b10_1010
`define FUNCT_SLTU 6'b10_1011
`define FUNCT_SLL  6'b00_0000
`define FUNCT_SRL  6'b00_0010
`define FUNCT_SRA  6'b00_0011
`define FUNCT_SLLV 6'b00_0100
`define FUNCT_SRLV 6'b00_0110
`define FUNCT_SRAV 6'b00_0111
`define FUNCT_JR   6'b00_1000

// ==========ALU操作控制信号定义==========

`define ALU_ADD  4'b0000
`define ALU_SUB  4'b0001
`define ALU_AND  4'b0010
`define ALU_OR   4'b0011
`define ALU_XOR  4'b0100 // 异或运算
`define ALU_NOR  4'b0101 // 或非运算
`define ALU_SLT  4'b0110 // 有符号比较
`define ALU_SLTU 4'b0111 // 无符号比较
`define ALU_SLL  4'b1000 // 逻辑左移
`define ALU_SRL  4'b1001 // 逻辑右移
`define ALU_SRA  4'b1010 // 算术右移
`define ALU_LUI  4'b1011 // 用于LUI指令

// ==========程序计数器初始地址==========

`define PC_INIT 32'h0000_0000

// ========指令存储器大小相关定义=========

`define IMEM_SIZE 1024        // 指令存储器大小（字数）
`define IMEM_ADDR_WIDTH 12    // 指令存储器地址宽度

// ========数据存储器操作宽度定义=========

`define MEM_WIDTH_BYTE   2'b00
`define MEM_WIDTH_HALF   2'b01
`define MEM_WIDTH_WORD   2'b10

/*
// ========数据存储器操作类型定义=========

`define MEM_OP_LBYTE        3'b000    // 有符号字节加载
`define MEM_OP_LHALF        3'b001    // 有符号半字加载
`define MEM_OP_LWORD        3'b010    // 字加载
`define MEM_OP_LBYTEU       3'b100    // 无符号字节加载
`define MEM_OP_LHALFU       3'b101    // 无符号半字加载
*/

`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module alu_control(
    input wire [1:0] alu_op,            // ALU操作类型指示（来自主控制单元）
    input wire [5:0] funct,             // 功能字段（用于R型指令）
    input wire [5:0] opcode,            // 指令操作码（用于I型指令）
    output reg [3:0] alu_ctrl           // ALU控制信号
);

    always @(*) begin
        case(alu_op)
            2'b00: alu_ctrl = `ALU_ADD;  // 用于lw/sw/addi等指令的加法操作
            
            2'b01: alu_ctrl = `ALU_SUB;  // 用于beq/bne等指令的减法操作
            
            2'b10: begin                 // R-type指令，根据funct字段解码
                case(funct)
                    // 算术运算指令 (R-Type) 这些指令对两个寄存器中的数据进行算术运算，结果存入第三个寄存器。
                        // ALU 操作：这些指令需要 ALU 执行 ADD 或 SUB 操作。
                    `FUNCT_ADD,`FUNCT_ADDU: alu_ctrl = `ALU_ADD;     // add $1, $2, $3	有符号加法	$1 = $2 + $3    addu $1, $2, $3	无符号加法	$1 = $2 + $3 (不检查溢出)
                    `FUNCT_SUB,`FUNCT_SUBU: alu_ctrl = `ALU_SUB;     // sub $1, $2, $3	有符号减法	$1 = $2 - $3    subu $1, $2, $3	无符号减法	$1 = $2 - $3 (不检查溢出)
                    
                    // 逻辑运算指令 (R-Type) 这些指令对两个寄存器中的数据进行按位逻辑运算。
                        // ALU 操作：这些指令需要 ALU 执行 AND, OR, XOR, NOR 操作。
                    `FUNCT_AND:             alu_ctrl = `ALU_AND;     // and $1, $2, $3	  按位与	$1 = $2 & $3
                    `FUNCT_OR:              alu_ctrl = `ALU_OR;      //  or $1, $2, $3	  按位或    $1 = $2 | $3
                    `FUNCT_XOR:             alu_ctrl = `ALU_XOR;     // xor $1, $2, $3	按位异或	$1 = $2 ^ $3
                    `FUNCT_NOR:             alu_ctrl = `ALU_NOR;     // nor $1, $2, $3	按位或非	$1 = ~($2 | $3)
                    
                    // 比较指令 (R-Type) 比较两个寄存器的值，根据结果设置目标寄存器为 1 或 0
                        // ALU 操作：这些指令需要 ALU 执行 SLT 或 SLTU 操作。
                    `FUNCT_SLT:             alu_ctrl = `ALU_SLT;     // slt $1, $2, $3	有符号小于置位	if ($2 < $3) $1=1 else $1=0 
                    `FUNCT_SLTU:            alu_ctrl = `ALU_SLTU;    // sltu $1, $2, $3	无符号小于置位	if ($2 < $3) $1=1 else $1=0
                    
                    // 移位指令 (R-Type) 对寄存器中的数据进行移位操作。
                        // 区别在于移位位数的来源（指令中的 shamt 字段或 rs 寄存器），这个选择由数据通路中的多路器解决，ALU 本身只负责执行移位。
                    `FUNCT_SLL,`FUNCT_SLLV: alu_ctrl = `ALU_SLL;     // sll $1, $2, 10	逻辑左移	$1 = $2 << 10 (用指令中的 shamt 字段指定位数)    sllv $1, $2, $3	变量逻辑左移	$1 = $2 << $3 (用另一个寄存器的值指定位数)
                    `FUNCT_SRL,`FUNCT_SRLV: alu_ctrl = `ALU_SRL;     // srl $1, $2, 10	逻辑右移	$1 = $2 >> 10   srlv $1, $2, $3	变量逻辑右移	 $1 = $2 >> $3	
                    `FUNCT_SRA,`FUNCT_SRAV: alu_ctrl = `ALU_SRA;     // sra $1, $2, 10	算术右移	$1 = $2 >>> 10 (保持符号位)     srav $1, $2, $3	变量算术右移	$1 = $2 >>> $3
                    
                    // JR指令的ALU操作不重要，设置默认值
                    `FUNCT_JR:              alu_ctrl = `ALU_ADD;
                    
                    default: alu_ctrl = `ALU_ADD;
                endcase
            end
            
            2'b11: begin                 // I-type指令，根据opcode解码
                case(opcode)
                    `OP_ANDI: alu_ctrl = `ALU_AND;   // andi $1, $2, 0xFF	与立即数	$1 = $2 & 0xFF (0扩展立即数)
                    `OP_ORI:  alu_ctrl = `ALU_OR;    // ori $1, $2, 0xFF	或立即数	$1 = $2 | 0xFF
                    `OP_XORI: alu_ctrl = `ALU_XOR;   // xori $1, $2, 0xFF	异或立即数	$1 = $2 ^ 0xFF (0扩展立即数)
                    `OP_SLTI: alu_ctrl = `ALU_SLT;   // slti $1, $2, 100	小于立即数置位	if ($2 < 100) $1=1 else $1=0 (有符号比较)
                    `OP_SLTIU: alu_ctrl = `ALU_SLTU; // sltiu $1, $2, 100	小于立即数置位	if ($2 < 100) $1=1 else $1=0 (无符号比较)
                    `OP_LUI:   alu_ctrl = `ALU_LUI;  // lui $1, 0x1234	加载高位立即数	$1 = 0x1234 << 16 (将16位立即数放到高16位，低16位为0)
                    default:   alu_ctrl = `ALU_ADD;
                endcase
            end
            
            default: alu_ctrl = `ALU_ADD;
        endcase
    end
endmodule
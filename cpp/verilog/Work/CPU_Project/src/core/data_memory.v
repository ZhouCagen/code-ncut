`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module data_memory(
    input wire clk,
    input wire rst_n,
    input wire mem_read_en,
    input wire mem_write_en,
    input wire [2:0] mem_op,  
    input wire [31:0] address,
    input wire [31:0] write_data,
    output reg [31:0] read_data
);

    // 存储器实现：1KB容量，按字节组织（1024 x 8bit）
    reg [7:0] mem [0:1023];
    
    integer i;

    initial begin   
        $readmemh("E:/Vivado/Work/CPU_Project/testbench/dmem_init.txt", mem);
    end
    // 同步写 - 只支持字写入
    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            for (i = 0; i < 1024; i = i + 1) begin
                mem[i] <=8'b0;
            end
        end else if (mem_write_en) begin
            // 只支持字写入(SW指令)
            mem[address] <= write_data[7:0];
            mem[address+1] <= write_data[15:8];
            mem[address+2] <= write_data[23:16];
            mem[address+3] <= write_data[31:24];
        end
    end 

    // 异步读 - 只支持字读取
    always @(*) begin
        if(mem_read_en) begin
            // 只支持字读取(LW指令)
            read_data = {mem[address+3], mem[address+2], mem[address+1], mem[address]};
        end else begin
            read_data = 32'b0;
        end          
    end
    /*
    // 同步写
    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            for (i = 0; i < 1024; i = i + 1) begin
                mem[i] <=8'b0;
            end
        end else if (mem_write_en) begin
            case(mem_op[1:0])
                `MEM_WIDTH_BYTE: begin
                    mem[address] <= write_data[7:0];
                end
                `MEM_WIDTH_HALF: begin
                    mem[address] <= write_data[7:0];
                    mem[address+1] <= write_data[15:8];
                end
                `MEM_WIDTH_WORD: begin
                    mem[address] <= write_data[7:0];
                    mem[address+1] <= write_data[15:8];
                    mem[address+2] <= write_data[23:16];
                    mem[address+3] <= write_data[31:24];
                end
            endcase
        end
    end 
    */

    /*
    // 异步读
    always @(*) begin
        if(mem_read_en) begin
            case(mem_op)
                `MEM_OP_LBYTE: begin     // 有符号字节加载
                  read_data = {{24{mem[address][7]}}, mem[address]};
                end
                `MEM_OP_LHALF: begin     // 有符号半字加载
                    read_data = {{16{mem[address+1][7]}}, mem[address+1], mem[address]};
                end
                `MEM_OP_LWORD: begin     // 字加载
                    read_data = {mem[address+3], mem[address+2], mem[address+1], mem[address]};
                end
                `MEM_OP_LBYTEU: begin    // 无符号字节加载
                    read_data = {24'b0, mem[address]};
                end
                `MEM_OP_LHALFU: begin    // 无符号半字加载
                    read_data = {16'b0, mem[address+1], mem[address]};
                end
                default: begin
                    read_data = 32'b0;
                end
            endcase
        end else begin
            read_data = 32'b0;
        end          
    end
    */

endmodule
`timescale 1ns/1ps

module register_file(
    input wire clk,
    input wire rst_n,
    input wire write_enable,
    input wire [4:0] read_addr1,
    input wire [4:0] read_addr2,
    input wire [4:0] write_addr,
    input wire [31:0] write_data,
    output wire [31:0] read_data1,
    output wire [31:0] read_data2
);

    reg [31:0] register_bank [0:31]; // 32个32位寄存器

    // 异步读：指读取操作不需要时钟信号触发，一旦地址输入发生变化，经过一定的组合逻辑延迟后，数据就会立即输出。
    assign read_data1 = (read_addr1 == 5'b0) ? 32'b0 : register_bank[read_addr1];
    assign read_data2 = (read_addr2 == 5'b0) ? 32'b0 : register_bank[read_addr2];

    // 同步写：指写入操作需要时钟信号触发，只有在时钟的有效边沿（通常是上升沿）到来时，并且满足特定条件（如写使能有效），数据才会被写入目标位置。

    integer i;
    // 低电平有效复位 (Active-Low Reset)
    always @(posedge clk or negedge rst_n) begin
        if(!rst_n) begin
            // 复位时将所有寄存器清零
            for(i = 0; i < 32; i = i + 1) begin
                register_bank[i] <= 32'b0;
            end    
        end else if(write_enable && write_addr != 5'b0) begin
            // 写使能有效且不是写$zero寄存器时，写入数据
            register_bank[write_addr] <= write_data;
        end
    end

endmodule        
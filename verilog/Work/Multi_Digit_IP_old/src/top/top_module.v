module top_module(
    input wire clk,
    input wire rst,
    input wire [3:0] N_sel,      // 选择N的值 (1-8)
    input wire [7:0] A_in,       // 输入A (最高8位)
    input wire [7:0] B_in,       // 输入B (最高8位)
    input wire [3:0] op_sel,     // 操作选择 (1-10)
    input wire execute,          // 执行按钮
    output wire [7:0] result,    // 结果显示
    output wire [2:0] status,    // 状态显示 (比较结果等)
    output wire [6:0] seg,       // 七段数码管段选
    output wire [3:0] an         // 七段数码管位选
);

    // 根据N_sel确定N的值
    wire [2:0] N;
    assign N = (N_sel == 0) ? 3'd7 : N_sel[2:0] - 1; // 如果N_sel为0，默认8位
    
    // 根据N的值截取A和B的输入（取最低N位）
    wire [7:0] A_trunc, B_trunc;
    assign A_trunc = A_in & ((1 << (N + 1)) - 1);
    assign B_trunc = B_in & ((1 << (N + 1)) - 1);
    
    // 操作结果
    wire [15:0] op_result;
    wire [2:0] cmp_result;
    
    // 实例化操作单元
    operation_unit op_unit(
        .clk(clk),
        .rst(rst),
        .execute(execute),
        .op_sel(op_sel),
        .A(A_trunc),
        .B(B_trunc),
        .N(N),
        .op_result(op_result),
        .cmp_result(cmp_result)
    );
    
    // 结果显示
    assign result = op_result[7:0];
    assign status = cmp_result;
    
    // 七段数码管显示
    seg7_display seg7(
        .clk(clk),
        .data(op_result),
        .seg(seg),
        .an(an)
    );

endmodule
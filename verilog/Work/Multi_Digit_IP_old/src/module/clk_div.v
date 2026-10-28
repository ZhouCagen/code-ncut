module clk_div(
    input wire clk,
    input wire rst,
    output reg [15:0] clk_div_cnt
);

always @(posedge clk or posedge rst) begin
    if(rst)
        clk_div_cnt <= 16'd0;
    else
        clk_div_cnt <= clk_div_cnt + 1;
end
    
endmodule
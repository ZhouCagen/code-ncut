module operation_unit(
    input wire clk,
    input wire rst,
    input wire execute,
    input wire [3:0] op_sel,
    input wire [7:0] A,
    input wire [7:0] B,
    input wire [2:0] N,
    output reg [15:0] op_result,
    output reg [2:0] cmp_result
);
    // 内部信号
    wire [7:0] add_sum, sub_diff, mul_product, counter_out, mux_out, dec_out;
    wire [7:0] bcd_add_sum, bcd_sub_diff;
    wire add_cout, sub_bout, bcd_add_cout, bcd_sub_bout;
    wire [2:0] cmp_out;
    wire parity_out;
    wire [255:0] dec_full_out;
    
    // 实例化各个功能模块，使用固定位宽
    addN_bin #(.N(8)) u_add(.A(A), .B(B), .Cin(1'b0), .Sum(add_sum), .Cout(add_cout));
    subN_bin #(.N(8)) u_sub(.A(A), .B(B), .Bin(1'b0), .Diff(sub_diff), .Bout(sub_bout));
    mulN_bin #(.N(8)) u_mul(.A(A), .B(B), .Product(mul_product));
    cmpN_bin #(.N(8)) u_cmp(.A(A), .B(B), .A_gt_B(cmp_out[2]), .A_eq_B(cmp_out[1]), .A_lt_B(cmp_out[0]));
    counterN_bin u_counter(.clk(clk), .rst(rst), .N(N), .Q(counter_out));
    even_parityN_bin #(.N(8)) u_parity(.A(A), .P(parity_out));
    muxN_bin #(.N(8)) u_mux(.A(A), .B(B), .Sel(B[0]), .Y(mux_out));
    decNto2N_bin #(.N(8)) u_dec(.A(A), .En(1'b1), .Y(dec_full_out));
    addN_bcd #(.N(8)) u_add_bcd(.A(A), .B(B), .Cin(1'b0), .Sum(bcd_add_sum), .Cout(bcd_add_cout));
    subN_bcd #(.N(8)) u_sub_bcd(.A(A), .B(B), .Bin(1'b0), .Diff(bcd_sub_diff), .Bout(bcd_sub_bout));
    
    // 只取译码器的低2^N位输出
    wire [255:0] dec_mask;
    assign dec_mask = (1 << (1 << (N + 1))) - 1;
    assign dec_out = (dec_full_out & dec_mask)[7:0];
    
    // 操作执行
    always @(posedge clk or posedge rst) begin
        if(rst) begin
            op_result <= 0;
            cmp_result <= 0;
        end else if(execute) begin
            case(op_sel)
                4'b0001: begin // 加法
                    op_result <= {8'b0, add_cout, add_sum[6:0]};
                end
                
                4'b0010: begin // 减法
                    op_result <= {8'b0, sub_bout, sub_diff[6:0]};
                end
                
                4'b0011: begin // 乘法
                    op_result <= {8'b0, mul_product};
                end
                
                4'b0100: begin // 比较
                    cmp_result <= cmp_out;
                    op_result <= {13'b0, cmp_out};
                end
                
                4'b0101: begin // 计数器
                    op_result <= {8'b0, counter_out};
                end
                
                4'b0110: begin // 奇偶校验
                    op_result <= {15'b0, parity_out};
                end
                
                4'b0111: begin // 多路选择器
                    op_result <= {8'b0, mux_out};
                end
                
                4'b1000: begin // 译码器
                    op_result <= {8'b0, dec_out};
                end
                
                4'b1001: begin // BCD加法
                    op_result <= {8'b0, bcd_add_cout, bcd_add_sum[6:0]};
                end
                
                4'b1010: begin // BCD减法
                    op_result <= {8'b0, bcd_sub_bout, bcd_sub_diff[6:0]};
                end
                
                default: op_result <= 0;
            endcase
        end
    end
endmodule

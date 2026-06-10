module counterN_bin #(parameter N = 8)(
    input wire clk,
    input wire rst,
    output wire [N-1:0] Q
);

    localparam Num_Blocks = N/2;
    genvar i;

    generate 
        for(i = 0 ;i < Num_Blocks; i = i + 1) begin
            counter2_bin(
                .clk(clk),
                .rst(rst),
                .Q(Q[2*i +: 2])
            );
        end
    endgenerate

endmodule    
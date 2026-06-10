module counter2_bin(
    input wire clk,
    input wire rst,
    output reg [1:0] Q
);

    always @(posedge clk or posedge rst) begin
        if(rst)
            Q <= 2'b00;
        else
            Q <= Q + 1;
    end        

endmodule
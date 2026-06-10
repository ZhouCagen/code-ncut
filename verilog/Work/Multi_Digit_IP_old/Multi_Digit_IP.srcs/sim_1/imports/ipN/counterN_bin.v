module counterN_bin #(parameter N = 8)(
    input wire clk,
    input wire rst,
    output wire [N-1:0] Q
);
    always @(posedge clk or posedge rst) begin
        if(rst)
            Q <= 0;
        else
            Q <= Q + 1;
    end       
    
endmodule
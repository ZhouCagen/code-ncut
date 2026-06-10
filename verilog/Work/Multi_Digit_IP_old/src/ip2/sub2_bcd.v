module sub2_bcd(
    input wire [3:0] A,
    input wire [3:0] B,
    input wire Bin,
    output reg [3:0] Diff,
    output reg Bout
);

    wire [4:0] temp;
    assign temp = {1'b0, A} - {1'b0, B} - {4'b0, Bin};
    
    always @(*) begin
        if (temp[4] == 1'b1) begin
            // 需要借位，进行BCD校正
            Bout = 1'b1;
            Diff = temp[3:0] + 4'd10;
        end else begin
            Bout = 1'b0;
            Diff = temp[3:0];
        end
    end

endmodule
module input_truncator #(
    parameter WIDTH = 8
)(
    input wire [WIDTH-1:0] data_in,
    input wire [2:0] N,
    output reg [WIDTH-1:0] data_out
);
    always @(*) begin
        if (N == 0)
            data_out = data_in;
        else
            data_out = data_in & ((1 << N) - 1);
    end
endmodule
module demux_2way(
    input wire D,
    input wire sel,
    output wire Y0,
    output wire Y1
);

    assign Y0 = D & ~sel;
    assign Y1 = D & sel;

endmodule
module alu_simple(
    input wire [3:0] A,
    input wire [3:0] B,
    input wire [2:0] ALU_Sel,
    output reg [3:0] Result,
    output reg       Cout
);

    always @(*) begin
        Cout =0;
        case(ALU_Sel)
            3'b000: {Cout, Result} = A + B;
            3'b001: {Cout, Result} = A - B;
            3'b010: Result = A & B;
            3'b011: Result = A | B;
            3'b100: Result = ~A;
            3'b101: Result = ~B;
            default: Result = 4'b0000;
            
        endcase    
    end
    
endmodule 
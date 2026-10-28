`timescale 1ns/1ps

module sign_extend_tb();

    reg [15:0] data_in_tb;
    wire [31:0] data_out_tb;

    sign_extend dut(
        .data_in(data_in_tb),
        .data_out(data_out_tb)
    );

    initial begin
        
        // 测试用例1：正数（最高位为0）
        data_in_tb = 16'h7FFF; // 最大正数
        #10;
        $display("Input = %h (Positive), Output = %h", data_in_tb, data_out_tb);

        // 测试用例2：负数（最高位为1）
        data_in_tb = 16'h8000; // 最小负数
        #10;
        $display("Input = %h (Negative), Output = %h", data_in_tb, data_out_tb);

        // 测试用例3：0
        data_in_tb = 16'h0000;
        #10;
        $display("Input = %h (Zero), Output = %h", data_in_tb, data_out_tb);

        // 测试用例4：-1
        data_in_tb = 16'hFFFF; // -1的补码表示
        #10;
        $display("Input = %h (-1), Output = %h", data_in_tb, data_out_tb);

        // 测试用例5：随机正数
        data_in_tb = 16'h1234;
        #10;
        $display("Input = %h (Random Positive), Output = %h", data_in_tb, data_out_tb);

        // 测试用例6：随机负数
        data_in_tb = 16'hABCD;
        #10;
        $display("Input = %h (Random Negative), Output = %h", data_in_tb, data_out_tb);

        // 结束仿真
        #10;
        $finish;
    end

endmodule    
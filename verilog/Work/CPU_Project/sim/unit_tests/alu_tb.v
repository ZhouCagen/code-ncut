`timescale 1ns/1ps

`include "E:\Vivado\Work\CPU_Project\src\defines.vh"

module alu_tb;
    // Define signals
    reg [31:0] a, b;
    reg [3:0] alu_control;
    wire [31:0] result;
    wire zero;
    
    // Instantiate the ALU under test
    alu uut (
        .a(a),
        .b(b),
        .alu_control(alu_control),
        .result(result),
        .zero(zero)
    );
    
    integer test_count;
    integer passed_tests;
    
    // Test task: Check results and report
    task check_test;
        input [31:0] expected_result;
        input expected_zero;
        input [100:0] test_name;
        begin
            test_count = test_count + 1;
            #10; // Wait for stability
            
            if (result === expected_result && zero === expected_zero) begin
                $display("PASS");
                passed_tests = passed_tests + 1;
            end else begin
                $display("FAIL");
                $display("   Expected: result=0x%h, zero=%b", expected_result, expected_zero);
                $display("   Got:      result=0x%h, zero=%b", result, zero);
            end
        end
    endtask
    
    // Test process
    initial begin
        // Initialize
        test_count = 0;
        passed_tests = 0;
        a = 0;
        b = 0;
        alu_control = 0;
        
        #10
        $display("Starting ALU testbench...");
        $display("----------------------------------------");
        
        // Test addition
        alu_control = `ALU_ADD;
        a = 32'h00000005;
        b = 32'h00000003;
        check_test(32'h00000008, 0, "ADD");
        
        a = 32'hFFFFFFFF;
        b = 32'h00000001;
        check_test(32'h00000000, 1, "ADD");
        
        // Test subtraction
        alu_control = `ALU_SUB;
        a = 32'h0000000A;
        b = 32'h00000003;
        check_test(32'h00000007, 0, "SUB");
        
        a = 32'h00000005;
        b = 32'h0000000A;
        check_test(32'hFFFFFFFB, 0, "SUB");
        
        // Test AND operation
        alu_control = `ALU_AND;
        a = 32'hF0F0F0F0;
        b = 32'h12345678;
        check_test(32'h10305070, 0, "AND");
        
        // Test OR operation
        alu_control = `ALU_OR;
        a = 32'hF0F0F0F0;
        b = 32'h0F0F0F0F;
        check_test(32'hFFFFFFFF, 0, "OR");
        
        // Test XOR operation
        alu_control = `ALU_XOR;
        a = 32'hAAAAAAAA;
        b = 32'h55555555;
        check_test(32'hFFFFFFFF, 0, "XOR");
        
        // Test NOR operation
        alu_control = `ALU_NOR;
        a = 32'h12345678;
        b = 32'h0000FFFF;
        check_test(~(32'h12345678 | 32'h0000FFFF), 0, "NOR");
        
        // Test signed comparison
        alu_control = `ALU_SLT;
        a = 32'hFFFFFFFE; // -2
        b = 32'h00000001; // 1
        check_test(32'h00000001, 0, "SLT");
        
        a = 32'h00000005; // 5
        b = 32'h00000003; // 3
        check_test(32'h00000000, 1, "SLT");
        
        // Test unsigned comparison
        alu_control = `ALU_SLTU;
        a = 32'hFFFFFFFE; // 4294967294
        b = 32'h00000001; // 1
        check_test(32'h00000000, 1, "SLTU");
        
        a = 32'h00000005; // 5
        b = 32'h0000000A; // 10
        check_test(32'h00000001, 0, "SLTU");
        
        // Test logical left shift
        alu_control = `ALU_SLL;
        a = 32'h00000004; // Shift amount
        b = 32'h0000000F; // Number to shift
        check_test(32'h000000F0, 0, "SLL");
        
        // Test logical right shift
        alu_control = `ALU_SRL;
        a = 32'h00000004; // Shift amount
        b = 32'hF000000F; // Number to shift
        check_test(32'h0F000000, 0, "SRL");
        
        // Test arithmetic right shift
        alu_control = `ALU_SRA;
        a = 32'h00000004; // Shift amount
        b = 32'hF000000F; // Number to shift (negative number)
        check_test(32'hFF000000, 0, "SRA");
        
        // Test LUI instruction
        alu_control = `ALU_LUI;
        b = 32'h00001234; // Low 16-bit immediate
        check_test(32'h12340000, 0, "LUI");
        
        // Test zero flag
        alu_control = `ALU_ADD;
        a = 32'h00000000;
        b = 32'h00000000;
        check_test(32'h00000000, 1, "Zero flag");
        
        // Test boundary case
        alu_control = `ALU_ADD;
        a = 32'h7FFFFFFF; // Maximum positive number
        b = 32'h00000001; // Add 1
        check_test(32'h80000000, 0, "Boundary");
        
        // Test undefined opcode (should return x)
        alu_control = 4'b1111; // Undefined opcode
        a = 32'h12345678;
        b = 32'h87654321;
        #10;
        if (result === 32'bx) begin
            $display("PASS: Undefined opcode returns x");
            passed_tests = passed_tests + 1;
        end else begin
            $display("FAIL: Undefined opcode should return x");
        end
        test_count = test_count + 1;
        
        // Test summary
        #10;
        $display("----------------------------------------");
        $display("Test Summary: %0d/%0d tests passed", passed_tests, test_count);
        
        if (passed_tests == test_count) begin
            $display("All tests passed!");
        end else begin
            $display("Some tests failed!");
        end
        
        $finish;
    end
    
    // Generate waveform file (for debugging)
    initial begin
        $dumpfile("alu_wave.vcd");
        $dumpvars(0, alu_tb);
    end
endmodule

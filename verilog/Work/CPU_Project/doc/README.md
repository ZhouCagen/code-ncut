Pipeline CPU 项目

一、项目概述
=========

这是一个基于 Verilog 实现的 5 级流水线 CPU 项目，支持 31 条指令，包含数据前递(Forwarding)和冒险检测(Hazard Detection)机制。项目采用模块化设计，具有良好的可读性和可扩展性。

二、项目结构说明
=========

CPU_Project/
│
├── src/                                # 源代码目录
│   ├── core/                           # 核心流水线模块
│   │   ├── pipeline_cpu.v              # 流水线CPU核心模块，集成所有子模块
│   │   ├── program_counter.v           # 程序计数器模块，负责指令地址生成
│   │   ├── instruction_memory.v        # 指令存储器，存储程序代码
│   │   ├── register_file.v             # 寄存器文件，包含32个通用寄存器
│   │   ├── alu.v                       # 算术逻辑单元，执行计算操作
│   │   ├── alu_control.v               # ALU控制单元，解析ALU操作码
│   │   ├── control_unit.v              # 主控制单元，生成所有控制信号
│   │   ├── data_memory.v               # 数据存储器，用于数据读写
│   │   ├── forwarding_unit.v           # 前递单元，解决数据冒险问题
│   │   └── hazard_detection_unit.v     # 冒险检测单元，处理控制冒险和加载-使用冒险
│   ├── lib/                            # 辅助模块/库
│   │   ├── mux_2x1.v                   # 2选1多路器，用于数据选择
│   │   ├── mux_3x1.v                   # 3选1多路器，用于数据前递选择
│   │   ├── sign_extend.v               # 符号扩展器，扩展立即数
│   │   └── d_flip_flop.v               # D触发器，用于构建流水线寄存器
│   ├── top.v                           # 顶层封装模块，只包含外部接口
│   └── defines.vh                       # 全局宏定义和参数
│
├── testbench/                          # 测试平台目录
│   ├── top_tb.v                        # 顶层模块测试平台
│   ├── pipeline_cpu_tb.v               # 核心CPU模块测试平台
│   ├── unit_tests/                     # 单元测试目录
│   │   ├── alu_tb.v                    # ALU单元测试：测试ALU的所有运算功能
│   │   ├── register_file_tb.v          # 寄存器文件单元测试：测试寄存器文件的读写功能
│   │   ├── program_counter_tb.v        # 程序计数器单元测试：测试程序计数器的功能
│   │   ├── instruction_memory_tb.v     # 指令存储器单元测试：测试指令存储器的读取功能
│   │   ├── data_memory_tb.v            # 数据存储器单元测试：测试数据存储器的读写功能
│   │   ├── control_unit_tb.v           # 控制单元单元测试：测试控制单元的信号生成
│   │   ├── alu_control_tb.v            # ALU控制单元单元测试：测试ALU控制单元的功能
│   │   ├── forwarding_unit_tb.v        # 前递单元单元测试：测试前递单元的冒险检测和前递决策
│   │   ├── hazard_detection_unit_tb.v  # 冒险检测单元单元测试：测试冒险检测单元的功能
│   │   ├── mux_2x1_tb.v                # 2选1多路器单元测试：测试2选1多路选择器的功能
│   │   ├── mux_3x1_tb.v                # 3选1多路器单元测试：测试3选1多路选择器的功能
│   │   ├── sign_extend_tb.v            # 符号扩展器单元测试：测试符号扩展器的功能
│   │   └── d_flip_flop_tb.v            # D触发器单元测试：测试D触发器的功能
│   ├── imem_init.txt                   # 指令存储器初始化文件
│   └── dmem_init.txt                   # 数据存储器初始化文件
│
├── sim/                                # 仿真脚本目录
│   └── run_sim.tcl                     # 仿真运行脚本
│
└── doc/                                # 文档目录
    └── README.md                       # 项目说明文档（本文档

三、模块详细说明
=========

1、核心模块 (src/core/)
-------------------

pipeline_cpu.v
- 功能：CPU核心模块，集成所有子模块，实现5级流水线架构
- 接口：时钟(clk)、复位(reset)、存储器接口等
- 内部包含流水线寄存器(IF/ID, ID/EX, EX/MEM, MEM/WB)

program_counter.v
- 功能：程序计数器，存储下一条指令的地址
- 接口：时钟(clk)、复位(reset)、分支目标地址(branch_target)等
- 在时钟上升沿更新PC值

instruction_memory.v
- 功能：指令存储器，存储程序代码
- 接口：地址输入(address)、指令输出(instruction)
- 使用$readmemh从文件初始化

register_file.v
- 功能：寄存器文件，包含32个32位通用寄存器
- 接口：读地址(read_reg1, read_reg2)、写地址(write_reg)、写数据(write_data)
- 支持两个读端口和一个写端口

alu.v
- 功能：算术逻辑单元，执行计算操作
- 接口：操作数(a, b)、操作码(alu_control)、结果(result)
- 支持加减乘除、逻辑运算、移位等操作

alu_control.v
- 功能：ALU控制单元，解析ALU操作码
- 接口：ALUOp(来自主控制单元)、功能码(funct)
- 输出ALU的具体操作控制信号

control_unit.v
- 功能：主控制单元，解析指令操作码
- 接口：操作码(opcode)
- 输出所有控制信号(RegWrite, MemWrite, ALUSrc等)

data_memory.v
- 功能：数据存储器，用于数据读写
- 接口：地址(address)、写数据(write_data)、读数据(read_data)
- 支持字节、半字和字访问

forwarding_unit.v
- 功能：前递单元，解决数据冒险问题
- 接口：源寄存器(rs, rt)、目标寄存器(EX/MEM_rd, MEM/WB_rd)
- 输出前递控制信号(forwardA, forwardB)

hazard_detection_unit.v
- 功能：冒险检测单元，处理控制冒险和加载-使用冒险
- 接口：当前指令信息、流水线状态
- 输出冒险处理信号(stall, flush)

2、库模块 (src/lib/)
----------------

mux_2x1.v
- 功能：2选1多路选择器
- 接口：两个输入(input0, input1)、选择信号(sel)、输出(out)

mux_3x1.v
- 功能：3选1多路选择器
- 接口：三个输入(input0, input1, input2)、选择信号(sel)、输出(out)

sign_extend.v
- 功能：符号扩展器，扩展立即数
- 接口：输入立即数(imm)、输出扩展后的立即数(ext_imm)

d_flip_flop.v
- 功能：D触发器，用于构建流水线寄存器
- 接口：时钟(clk)、复位(reset)、输入(d)、输出(q)

3、顶层模块 (src/)
--------------

top.v
- 功能：顶层封装模块，只包含外部接口
- 接口：时钟(clk)、复位(reset)、存储器总线接口等
- 内部实例化pipeline_cpu模块

defines.v
- 功能：全局宏定义和参数
- 内容：指令操作码定义、ALU操作码定义、寄存器编号等

4、测试平台 (testbench/)
-------------------

tb_top.v
- 功能：顶层模块测试平台
- 内容：实例化top模块，生成时钟和复位信号，提供测试激励

tb_pipeline_cpu.v
- 功能：核心CPU模块测试平台
- 内容：实例化pipeline_cpu模块，用于调试内部信号

unit_tests/ - 单元测试目录
tb_alu.v: ALU单元测试 - 测试ALU的所有运算功能
tb_register_file.v: 寄存器文件单元测试 - 测试寄存器文件的读写功能
tb_program_counter.v: 程序计数器单元测试 - 测试程序计数器的功能
tb_instruction_memory.v: 指令存储器单元测试 - 测试指令存储器的读取功能
tb_data_memory.v: 数据存储器单元测试 - 测试数据存储器的读写功能
tb_control_unit.v: 控制单元单元测试 - 测试控制单元的信号生成
tb_alu_control.v: ALU控制单元单元测试 - 测试ALU控制单元的功能
tb_forwarding_unit.v: 前递单元单元测试 - 测试前递单元的冒险检测和前递决策
tb_hazard_detection_unit.v: 冒险检测单元单元测试 - 测试冒险检测单元的功能
tb_mux_2x1.v: 2选1多路器单元测试 - 测试2选1多路选择器的功能
tb_mux_3x1.v: 3选1多路器单元测试 - 测试3选1多路选择器的功能
tb_sign_extend.v: 符号扩展器单元测试 - 测试符号扩展器的功能
tb_d_flip_flop.v: D触发器单元测试 - 测试D触发器的功能

imem_init.txt
- 功能：指令存储器初始化文件
- 格式：每行一个32位十六进制数，表示一条指令

dmem_init.txt
- 功能：数据存储器初始化文件
- 格式：每行一个32位十六进制数，表示一个数据字

5、仿真脚本 (sim/)
-------------

run_sim.tcl
- 功能：仿真运行脚本
- 内容：自动化编译、仿真和波形查看流程

四、使用说明
=========

1. 编译项目：
   cd sim
   source run_sim.tcl

2. 运行仿真：
   vsim -do run_sim.tcl

3. 查看波形：
   仿真完成后，使用波形查看器分析信号

五、指令集支持
=========

本项目支持31条指令，包括：
- 算术指令：ADD, SUB, ADDI, etc.
- 逻辑指令：AND, OR, XOR, etc.
- 存储器访问指令：LW, SW
- 分支指令：BEQ, BNE
- 跳转指令：J, JR, JAL
- 其他指令：SLT, SLTI, etc.

六、流水线架构
=========

CPU采用5级流水线架构：
1. IF - 取指阶段：从指令存储器读取指令
2. ID - 译码阶段：解析指令，读取寄存器
3. EX - 执行阶段：执行计算操作
4. MEM - 访存阶段：访问数据存储器
5. WB - 写回阶段：将结果写回寄存器

七、冒险处理
=======

1. 数据冒险：通过前递(Forwarding)技术解决
2. 控制冒险：通过流水线刷新(Flush)解决
3. 加载-使用冒险：通过流水线停顿(Stall)解决

八、扩展性
=====

本项目采用模块化设计，易于扩展：
1. 添加新指令：在control_unit.v和alu_control.v中添加支持
2. 添加新功能：通过添加新模块或扩展现有模块实现
3. 优化性能：通过改进前递和冒险检测逻辑提高性能

九、单元测试指南
===========

每个单元测试文件应包含以下部分:
1. 测试模块声明: 定义测试模块
2. 被测模块实例化: 实例化待测试的模块
3. 测试信号定义: 定义测试所需的输入输出信号
4. 测试激励生成: 使用initial块生成测试激励
5. 预期结果检查: 使用assertion或$display检查结果
6. 波形输出: 使用$dumpfile和$dumpvars生成波形文件
7. 测试完成标志: 使用$finish结束测试

示例单元测试结构:
```verilog
module tb_example;
    // 信号定义
    reg clk, reset;
    wire [31:0] result;
    
    // 实例化被测模块
    example_module uut (.clk(clk), .reset(reset), .result(result));
    
    // 时钟生成
    always #5 clk = ~clk;
    
    // 测试激励
    initial begin
        clk = 0; reset = 1;
        #10 reset = 0;
        
        // 添加测试用例
        
        #100 $finish;
    end
    
    // 波形记录
    initial begin
        $dumpfile("tb_example.vcd");
        $dumpvars(0, tb_example);
    end
    
    // 结果检查
    always @(posedge clk) begin
        // 添加断言或结果检查
    end
endmodule




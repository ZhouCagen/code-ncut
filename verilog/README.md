# Verilog 设计与仿真源文件

本目录仅保留人工维护的 HDL 源码、testbench、测试向量、上板约束文件以及少量用于仿真的波形配置；Vivado 自动生成的编译、仿真、综合结果不纳入版本控制。

## 目录

| 目录 | 内容 |
| --- | --- |
| `login_lib/src/` | 基础 / 扩展 / 常用组合时序模块 |
| `login_lib/sim/` | 对应模块的 testbench |
| `Work/CPU_Project/src/` | 五级流水线 CPU 设计及宏定义 |
| `Work/CPU_Project/testbench/` | CPU 单元仿真、整体仿真、存储器测试数据 |
| `Work/Multi_Digit_IP_old/src/` | 多位运算和板级模块 |
| `Work/Multi_Digit_IP_old/sim/` | 运算模块的 testbench |
| `Work/Student ID Display System/` | 学号数码管显示：src、sim、constraints |
| `Work/Student Name Display System/` | 姓名显示：src、sim、constraints |

`constraints/` 中的 .xdc 是上板引脚约束，**并非缓存**。CPU 的 `pipeline_cpu_tb_behav.wcfg` 是波形窗口配置，也保留了。

## 用 Vivado 打开

新建 RTL Project 后手动加入目标项目 `src/` 下的 `.v/.vh`，将 `sim/` 或 `testbench/` 的 `.v` 加入 Simulation Sources，按需加入 `constraints/` 的 `.xdc`。例如 CPU 的 `src/defines.vh` 需要设置为 include 文件，并把其所在目录设置为仿真/综合的 include 搜索路径。

**注意：本次只是仓库文件清理，没有修改或验证 RTL 功能。** CPU 的现有源码含旧 Windows 绝对 `include` 路径，重新仿真前仍需修正；部分 testbench 只打印结果，未做自动断言。此前的 `.xpr` 及自动生成目录不再保留在清理后的分支中，但可以从 Git 历史恢复。

学号显示项目的 RTL / testbench / XDC 已从 Vivado 的 `.srcs/` 导出到 `src/` / `sim/` / `constraints/`。多位运算的 `even_parityN_bin.v` 保留了此前 `.srcs` 中引用 `even_parity2_bin` 的版本，以避免 `src` 旧版本自实例化。

**今后在 Vivado GUI 创建源码时，请将原始设计同步到本目录的 `src/`、`sim/`，不要把 `.srcs/` 当作唯一备份。**

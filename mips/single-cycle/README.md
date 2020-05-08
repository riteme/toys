# Assignment 1 实验报告

## 薛振梁 18307130172

### 概况

单周期 MIPS 指令集 CPU，实现了以下指令[^ref]：

```
lw		$rt, imm($rs)
sw		$rt, imm($rs)
add		$rd, $rs, $rt
sub		$rd, $rs, $rt
and		$rd, $rs, $rt
xor		$rd, $rs, $rt
nor		$rd, $rs, $rt
slt		$rd, $rs, $rt
addi	$rt, $rs, imm
andi	$rt, $rs, imm
ori		$rt, $rs, imm
xori	$rt, $rs, imm
slti	$rt, $rs, imm
j		addr
beq		$rs, $rt, imm
bne		$rs, $rt, imm
jr		$rs
sll		$rd, $rt, shamt
srl		$rd, $rt, shamt
sra		$rd, $rt, shamt
jal		addr
```

CPU 原理图总图：

![](assets/schematic.svg)

### 数据路径分析

<st>`PC` 更新</st>每一个时钟周期完成以后，下一个时钟周期的新的 `PC'` 一共有四种情况：

1. 正常情况：`PC'` 指向下一条指令，此时 `PC'` = `PC` + 4。
2. 直接跳转：由 `j` 指令触发，`PC'` 直接变为 `j` 指令中指定的地址。
3. 条件跳转：由 `beq` 和 `bne` 指令触发，指令中只包含地址的偏移量，`PC'` 变为 `PC` + 4 + 偏移量。是否实际发生跳转需要根据 ALU 的计算结果来判断。
4. 寄存器跳转：由 `jr` 指令触发，`PC'` 变为指定寄存器的内容。

使用一个四路复用器和控制信号 `PCSelect` 实现。`PC` 本身使用一个寄存器存储，并在时钟上升沿时触发写入。

<st>指令解析</st>`PC` 传给指令集内存后，读取到的数据将被分为多个分量传给不同的地方。其中对于控制器只需要知道 `opcode`（第 31 至 26 位） 和 `funct`（第 5 至最后一位）即可。`rs` 分量（第 25 至 21 位）送至寄存器文件的 `A1` 端口，`rt` 分量（第 20 至 16 位）送至寄存器文件的 `A2` 端口。对于寄存器文件的写入端口，由于在 MIPS 指令集中，R 型指令是以 `rd` 作为目标寄存器，而其他指令使用 `rt` 作为目标寄存器，因此使用一个控制信号 `RegisterDestination`。

`shamt` 分量（第 10 至 6 位）送至 ALU。I 型指令的 `imm` 分量（第 15 至最后一位）需要进行符号扩展，其中对于逻辑运算，采用补 0 的扩展，对于其他运算，采用补充最高位的符号扩展，这里需要使用 `SignExtension` 信号来控制。J 型指令的 `addr` 分量经 JTA Extension 模块加工后直接送至 `PCSelect` 处。

<st>ALU</st>以 `A`、`B` 两个端口作为操作数的输入，由 `OP` 端口控制具体的运算。`OP` 的编码与 R 型指令中的 `funct` 保持一致。`RET` 表示计算结果，一般情况下是 `RET = A OP B`，其中 `B` 既可能是寄存器 `rt` 的值，也可能是立即数，具体行为由信号 `ALUSourceB` 控制。对于移位运算，`RET = B OP A`，此时 `A` 端口要使用控制信号 `UseShift` 切换到 `shamt`。MIPS 指令集在这里特意使用 `rt` 作为操作数，使得 ALU 前两个端口的控制能够并发处理，减少一定的延时。

为了支持 `beq` 和 `bne`，另外定义了两个 `OP`（`111111` 和 `111110`），此时 ALU 的 `Z` 端口会根据具体的情况判断输入 `A` 和 `B` 是否相等，并且输出 0 或 1。这个输出结果会和控制信号 `PCSelect` 按位与。如果 `Z` 输出 0，则会强制 `PCSelect` 为 `00`（`PC` 更新的正常情况）。在其他的 `OP` 的情况下，`Z` 只输出 1。

<st>数据交互</st>所有数据存储器的读取数据均为组合逻辑，写入数据有对应的控制信号控制，并且需要由时钟周期上升沿触发。内存与寄存器文件之间的数据交互只有 `lw` 和 `sw` 两个指令完成。对于 `lw`，由 ALU 计算地址，并将内存输出直接送至寄存器文件的写入 `WD` 处。对于 `sw`，同样由 ALU 计算地址，并将寄存器 `rt` 的值送至内存的写入 `WD` 处。由于寄存器写回还有其他的计算指令，因此需要一个控制信号 `ResultSource` 来决定。

<st>`jal` 指令</st>`jal` 指令需要将 `PC` + 4 写入 `$ra` 寄存器。这里使用一个控制信号 `SavePC` 提供该功能，即操作寄存器文件的 `A3` 和 `WD` 输入。

<st>控制器</st>上述所有控制信号均放入一个 16 位的总线中。控制器只需要对不同的 `opcode` 和 `funct` 确定对应的控制信号即可。

### 测试

主要使用由 GCC 交叉编译到 MIPS32 架构的汇编代码进行测试，使用 verilator[^verilator][^zipcpu] 搭建测试框架。在测试的时候发现 GCC 生成的汇编代码有时候会把跳转指令（如 `j`，`beq` 之类）和前一条的位置互换，后来查阅资料发现这是实际的 MIPS 流水线架构有 “延迟跳转” 的缺陷导致的[^tips]。延迟跳转即跳转指令的后一条指令一定会被执行，因此 GCC 默认情况下会尝试调整指令的顺序，将原本应在跳转前的指令放置在跳转后，从而避免插入一条 `nop` 指令。这个特性可以使用 `-fno-delayed-branch` 关闭[^no-delay]，从而方便使用 GCC 获得汇编代码。

最终测试了一个数论算法[^dyh]，其中整数乘法和整数带余除法都是软实现的。这个测试包含了数组操作和函数递归调用，以及测试了整型溢出的情况。

### 参考资料

[^verilator]: "Verilator", <https://www.veripool.org/wiki/verilator>.
[^zipcpu]: "Verilog, Formal Verification and Verilator Beginner's Tutorial", The ZipCPU by Gisselquist Technology, <https://zipcpu.com/tutorial/>.

[^ref]: "MIPS32® Instruction Set Quick Reference", <https://s3-eu-west-1.amazonaws.com/downloads-mips/documents/MD00565-2B-MIPS32-QRC-01.01.pdf>.
[^tips]: "C to MIPS compiler", <http://reliant.colab.duke.edu/c2mips/>.
[^no-delay]: "How to turn off MIPS-GCC automatic instruction reordering?", Stack Overflow, <https://stackoverflow.com/questions/4042517/how-to-turn-off-mips-gcc-automatic-instruction-reordering>.
[^dyh]: "杜教筛", OI Wiki, <https://oi-wiki.org/math/du/>.
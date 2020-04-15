### Formats

`opcode [31:26]`, `rs [25:21]`, `rt [20:16]`

* **R-type:** `opcode`=0 (6), `rs` (5), `rt` (5), `rd` (5), `shamt` (5), `funct` (6)
  * `rd [15:11]`
  * `shamt [10:6]`
  * `funct [5:0]`
* **I-type:** `opcode` (6),  `rs` (5), `rt` (5), `imm` (16)
  * `imm [15:0]`
* **J-type:** `opcode` (6), `addr` (26)
  * `addr [25:0]`

<br>

### Conventions

**SignImm:** sign-extended 16-bit immediates.

**ZeroImm:** zero-extended 16-bit immediates.

**BTA extension:** `PC + 4` with offset sign-extended 4 `imm`.

**JTA extension:** `(PC+4)[31:28]` (4), `addr` (26), `00`. No extension for `addr`.

<br>

### Instructions

```asm
lw		$rt, imm($rs)
sw		$rt, imm($rs)
add		$rd, $rs, $rt
sub		$rd, $rs, $rt
and		$rd, $rs, $rt
or      $rd, $rs, $rt
xor		$rd, $rs, $rt
nor		$rd, $rs, $rt
slt		$rd, $rs, $rt
addi    $rt, $rs, imm
andi    $rt, $rs, imm
ori		$rt, $rs, imm
xori    $rt, $rs, imm
slti    $rt, $rs, imm
j		addr
beq		$rs, $rt, imm
bne		$rs, $rt, imm
jr		$rs
sll		$rd, $rt, shamt
srl		$rd, $rt, shamt
sra		$rd, $rt, shamt
jal		addr
```

<br>

### Instruction Details

`add`, `sub`, `and`, `or`, `xor`, `nor`: R; (`100000`, `100010`, `100100`, `100101`, `100110`, `100111`), `rd` = `rs` op `rt`.

`slt`: R; (`101010`) if `rs` < `rt`, `rd` = 1, signed-comparison.

`addi`, `andi`, `ori`, `xori`: I; (`1000`, `1100`, `1101`, `1110`) `rt` = `rs` + SignImm (`addi`) / ZeroImm (logical)

`slti`: I; (`1010`), `rt` as destination, compare `rs` with SignImm.

`sw`, `lw`: I; (`101011`, `100011`), `rt` & `imm(rs)`, SignImm.

`j`: J; (`10`), JTA extension.

`nop`: *equivalent to* `sll $0, $0, 0`.

`beq`, `bne`: I; (`100`, `101`) compare `rs` & `rt`, BTA extension.

`jal`: J; (`11`), JTA extension, save next `PC` to `$ra`.

`jr`: R; (`1000`) `PC` = `rs`.

`sll`, `srl`, `sra`: R; (`00`, `10`, `11`) `rd` = `rt` op `shamt`.

<br>

### Stages

* `lw rt, imm(rs)`
```
Decode:
    vs ← R[rs]
    imm ← SignExtension(imm)

ALU:
    addr ← vs + imm

Memory:
    data ← M[addr]

Store:
    R[rd] ← data
```

* `sw rt, imm(rs)`
```
Decode:
    vs, vt ← R[rs], R[rt]
    imm ← SignExtension(imm)

ALU:
    addr ← vs + imm

Memory:
    M[addr] ← vt
```

* `¤ rd, rs, rt`
```
Decode:
    vs, vt ← R[rs], R[rt]

ALU:
    out ← vs ¤ vt

Memory:
	data ← out

Store:
    R[rd] ← ret
```

* `¤i rt, rs, imm`
```
Decode:
    vs ← R[rs]
    rd ← rt
    if ¤ is logical operation
        imm ← ZeroExtension(imm)
    else
        imm ← SignExtension(imm)

ALU:
    out ← vs ¤ imm

Memory:
	data ← out

Store:
    R[rd] ← data
```

* `s¤ rd, rt, shamt`
```
Decode:
    vt ← R[rt]

ALU:
    out ← sht ¤ rt

Memory:
	data ← out

Store:
    R[rd] ← data
```

* `j addr`
```
PC ← JTAExtension(addr)
```

* `jr rs`
```
Decode:
    PC ← R[rs]
```

* `jal addr`
```
Decode:
    PC ← JTAExtension(addr)
    imm ← PC + 1
    rd ← $ra

ALU:
	out ← imm

Memory:
	data ← out

Store:
	R[rd] ← data

# ALU op: b + 0
```

* `b¤ rs, rt, imm`
```
Decode:
    vs, vt = R[rs], R[rt]
    if vs ¤ vt
        PC ← BTAExtension(imm)
    else
        PC ← PC + 1
```

<br>

### Microinstructions

* Fetch
```
PC ← new PC
```

* Decode
```
vs ← R[rs]
vt ← R[rt]
imm ← Extension(imm_)
PC ← new PC
PC ← R[rs]
```

* ALU
```
ret ← A ¤ B
```

* Memory
```
M[addr] ← data
data ← M[addr]
```

* Store
```
R[rd] ← data
```

<br>

### Hazards

```
R[rd] ← data

    vs

vs/vt ← R[rs]/R[rt]
PC ← R[rs]
```

Writing to or reading from `$0` register do not result in data dependencies.

<br>

Jumps:

* `jr` always flush fetch register.

### Frontend

Goal: ensure no flush to fetch-stage register (by dynamic inserting `nop` instructions). Ability to swap adjacent instructions. Simple branch prediction.



#### Misc

Register `$0` is hardcoded. Everything wrote to `$0` is discarded. Assign `$0` to `rd` actually disable register update.
#!/usr/bin/env python3

from argparse import ArgumentParser

pr = ArgumentParser()
pr.add_argument('file')
args = pr.parse_args()

def parse_op(op):
    _ = {
        '000000': "(rtype)",
        '001000': 'addi',
        '001100': 'andi',
        '001101': 'ori',
        '001110': 'xori',
        '001010': 'slti',
        '101011': 'sw',
        '100011': 'lw',
        '000010': 'b',
        '000100': 'beq',
        '000101': 'bne',
        '000011': 'jal'
    }
    return _[op] if op in _ else "(unknown)"

def parse_funct(funct):
    _ = {
        '000000': 'sll',
        '000010': 'srl',
        '000011': 'sra',
        '001000': 'j',
        '101010': 'slt',
        '100000': 'add',
        '100010': 'sub',
        '100100': 'and',
        '100101': 'or',
        '100110': 'xor',
        '100111': 'nor'
    }
    return _[funct] if funct in _ else "(unknown)"

def parse_register(r):
    r = int(r, base=2)
    names = [
        '0', 'at', 'v0', 'v1', 'a0', 'a1', 'a2', 'a3',
        't0', 't1', 't2', 't3', 't4', 't5', 't6', 't7',
        's0', 's1', 's2', 's3', 's4', 's5', 's6', 's7',
        't8', 't9', 'k0', 'k1', 'gp', 'sp', 'fp', 'ra'
    ]
    return f'${r}/${names[r]}'

def parse_imm(imm):
    value = int(imm, base=2)
    neg = value - 2**len(imm) if imm[0] == '1' else value
    return f'{value}/{neg}'

def parse(line, pc):
    op, rs, rt, rd, shamt, funct = [
        line[0:6], line[6:11],
        line[11:16], line[16:21],
        line[21:26], line[26:32]
    ]
    imm = line[16:32]
    addr = line[6:32]

    print(f'pc = {pc}')

    R = '{:10s} {:8s} {:8s} {:8s} {:5s} {:10s}'
    print(R.format(op, rs, rt, rd, shamt, funct))
    print(R.format(
        parse_op(op), parse_register(rs), parse_register(rt),
        parse_register(rd), str(int(shamt, base=2)), parse_funct(funct)
    ))

    I = '{:10s} {:8s} {:8s} {:16s}'
    print(I.format(op, rs, rt, imm))
    print(I.format(
        parse_op(op), parse_register(rs),
        parse_register(rt), parse_imm(imm)
    ))

    J = '{:10s} {:26s}'
    print(J.format(op, addr))
    print(J.format(parse_op(op), parse_imm(addr)))

    print("")

with open(args.file, 'r') as fp:
    pc = 0
    for line in fp:
        line = line.strip()
        if len(line) == 0:
            continue
        parse(line, pc)
        pc += 4
#!/usr/bin/env python3

from pathlib import Path
from argparse import ArgumentParser


DOLLAR = '__dollar__'


def tokenize(fp):
    ret = []
    tags = []

    lineos = 0
    for line in fp:
        lineos += 1
        line = line.strip()
        if len(line) == 0:
            continue

        # print(f'> {line}')

        if line.endswith(':'):
            tags.append(line[:-1].replace('$', DOLLAR))
        else:
            if '#' in line:
                line, _ = line.split('#')

            parts = line.split()
            instr = parts[0]
            args = None
            if len(parts) > 1:
                args = parts[1]

            ret.append([
                instr.strip(),
                args.strip().split(',')
                if args else [],
                tags,
                lineos
            ])
            tags = []

    return ret


def preprocess(tokens, pc_start, addr_start):
    pc, addr = pc_start, addr_start
    tag_map = {}
    code, data = [], []

    _lineos = None
    def emits(obj, lineos=None):
        nonlocal pc
        nonlocal _lineos

        if lineos:
            _lineos = lineos

        for i, entry in enumerate(obj):
            code.append(entry + [_lineos, pc + 4 * i])

        pc += len(obj) * 4

    def emit(obj, lineos=None):
        emits([obj], lineos)

    # Polyfills (pseudo-instructions)

    at = '$1'  # assembler temporary

    def bltz(rs, t):
        emits([
            ['slt', [at, rs, '$0']],
            ['bne', [at, '$0', t]]
        ])

    def addu(*args):
        emit(['add', args])

    def addiu(*args):
        emit(['addi', args])

    def lui(rt, imm):
        emits([
            ['addi', [rt, '$0', imm]],
            ['sll', [rt, rt, '16']]
        ])

    def move(rt, rs):
        emit(['add', [rt, rs, '$0']])

    def li(rt, imm):
        lui(rt, f'%hi({imm})')
        emit(['addi', [rt, '$0', f'%lo({imm})']])

    # END OF Polyfills

    has_error = False
    for token, args, tags, lineos in tokens:
        if token.startswith('.'):
            for tag in tags:
                tag_map[tag] = addr

            data.append([token, args, lineos, addr])

            if token == '.word':
                addr += 4
            elif token == '.fill':
                addr += int(args[0]) * 4
            else:
                has_error = True
                print(f'(error) Unknown directive "{token}" at line {lineos}.')
        else:
            for tag in tags:
                tag_map[tag] = pc

            if token in locals() and callable(handler := locals()[token]):
                _lineos = lineos
                handler(*args)
            else:
                emit([token, args], lineos)

    if has_error:
        exit(-1)

    return code, data, tag_map


# Utilities
def cat(*args):
    ret = ''.join(args)
    # print(ret)
    return int(ret, base=2)

def idx(r):
    if r == 0:
        return '00000'
    return '{:05b}'.format(int(r[1:]))

def extract(pat):
    imm, _rs = pat.split('(')
    return int(imm), _rs[:-1]

def bi(imm, n):
    # if imm < 0:
    #     imm += 2**31
    imm &= 0xFFFFFFFF
    ret = bin(imm)[2:]
    if len(ret) > n:
        ret = ret[len(ret) - n:len(ret)]
    return '0' * (n - len(ret)) + ret

def translate(tokens, mp):
    # Instructions

    _pc = 0

    def nop():
        return bi(0, 32)

    def _lwsw(op, rt, pat):
        imm, rs = extract(pat)
        return cat(op, idx(rs), idx(rt), bi(imm, 16))
    def lw(rt, pat):
        return _lwsw('100011', rt, pat)
    def sw(rt, pat):
        return _lwsw('101011', rt, pat)

    def _rtype(funct, rd, rs, rt, shamt='0'):
        return cat('000000', idx(rs), idx(rt), idx(rd), bi(int(shamt), 5), funct)
    def add(*a):
        return _rtype('100000', *a)
    def sub(*a):
        return _rtype('100010', *a)
    def and_(*a):
        return _rtype('100100', *a)
    def or_(*a):
        return _rtype('100101', *a)
    def xor_(*a):
        return _rtype('100110', *a)
    def nor(*a):
        return _rtype('100111', *a)
    def slt(*a):
        return _rtype('101010', *a)
    def sll(rd, rt, shamt):
        return _rtype('000000', rd, 0, rt, shamt)
    def srl(rd, rt, shamt):
        return _rtype('000010', rd, 0, rt, shamt)
    def sra(rd, rt, shamt):
        return _rtype('000011', rd, 0, rt, shamt)

    def _itype(op, rt, rs, imm, offset=0, sht=0):
        attr = None
        if imm.startswith('%'):
            attr, imm = imm[:3], imm[3:]

        imm = eval(imm.replace('$', DOLLAR), None, mp)
        if attr == '%hi':
            imm = imm >> 16
        elif attr == '%lo':
            imm = imm & 0xFFFF
        # print(attr, imm)
        imm = (imm - offset) >> sht

        return cat(op, idx(rs), idx(rt), bi(imm, 16))

    def addi(*a):
        return _itype('001000', *a)
    def andi(*a):
        return _itype('001100', *a)
    def ori(*a):
        return _itype('001101', *a)
    def xori(*a):
        return _itype('001110', *a)
    def slti(*a):
        return _itype('001010', *a)
    def beq(rs, rt, imm):
        return _itype('000100', rt, rs, imm, offset=_pc + 4, sht=2)
    def bne(rs, rt, imm):
        return _itype('000101', rt, rs, imm, offset=_pc + 4, sht=2)

    def _jtype(op, addr):
        # print(addr)
        try:
            addr = int(addr)
        except:
            addr = mp[addr]
        addr = addr >> 2

        return cat(op, bi(int(addr), 26))

    def b(addr):  # actual j
        return _jtype('000010', addr)
    def jal(addr):
        return _jtype('000011', addr)

    def j(rs):  # actual jr
        return cat('000000', idx(rs), '00000' * 3, '001000')

    # END OF Instructions

    mem = []

    has_error = False
    for token, args, lineos, addr in tokens:
        assert addr % 4 == 0
        pos = addr >> 2
        if len(mem) < pos:
            mem += [0] * (pos - len(mem))

        if token.startswith('.'):
            if token == '.word':
                mem.append(int(args[0]))
            elif token == '.fill':
                repeat = int(args[0])
                value = int(args[1])
                mem += [value] * repeat
        else:
            L = locals()

            if token in L:
                handler = L[token]
            elif (name := f'{token}_') in L:
                handler = L[name]
            else:
                handler = None

            if handler is None or not callable(handler):
                print(f'(error) Unknown instruction "{token}" at line {lineos}.')
                mem.append(0)
            else:
                _pc = addr
                ret = handler(*args)
                if type(ret) != int:
                    ret = int(ret)
                mem.append(ret)

    if has_error:
        exit(-1)
    return mem


def dump(mem, fp):
    for word in mem:
        # fp.write('{:08x}\n'.format(word))
        fp.write('{:032b}\n'.format(word))


if __name__ == '__main__':
    pr = ArgumentParser()
    pr.add_argument('file', help="MIPS assembly file generated by godbolt.")
    pr.add_argument('-i', '--imem', default='code.hex', help="Text output file.")
    pr.add_argument('-d', '--dmem', default='data.hex', help="Data output file.")
    pr.add_argument('-pc0', default=0, type=int, help="initial PC.")
    pr.add_argument('-addr0', default=4, type=int, help="start address for data.")
    pr.add_argument('-v', '--verbose', action="store_true", help="show more messages.")
    pr.add_argument('-t', '--tags', action="store_true", help="Print tag positions.")
    args = pr.parse_args()

    if not Path(args.file).exists():
        print(f'File {args.file} does not exist.')
        exit(-1)

    with open(args.file, 'r') as fp:
        tokens = tokenize(fp)
    if args.verbose:
        print(tokens)

    code, data, mp = preprocess(tokens, pc_start=args.pc0, addr_start=args.addr0)
    if args.verbose:
        print(code, data)

    imem = translate(code, mp)
    dmem = translate(data, mp)
    if args.verbose:
        print(imem, dmem)

    with open(args.imem, 'w') as fp:
        dump(imem, fp)
    with open(args.dmem, 'w') as fp:
        dump(dmem, fp)

    if args.verbose or args.tags:
        print(mp)
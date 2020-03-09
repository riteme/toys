        b       main
sum:
        bltz    $4,$L4
        nop

        lui     $2,%hi(a)
        addiu   $2,$2,%lo(a)
        sll     $4,$4,2
        lui     $5,%hi(a-4)
        addu    $4,$2,$4
        addiu   $5,$5,%lo(a-4)
        move    $2,$0
$L3:
        lw      $3,0($4)
        addiu   $4,$4,-4
        addu    $2,$2,$3
        bne     $4,$5,$L3
        nop

        addiu   $2,$2,1
        j       $31
        nop

$L4:
        li      $2,1                        # 0x1
        j       $31
        nop

main:
        li      $4,4
        jal     sum
        sw      $2,0($0)
        nop

a:
        .word   1
        .word   2
        .word   3
        .word   4
        .word   5
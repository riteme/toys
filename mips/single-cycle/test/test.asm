addi $16,$0,a
lw $8,0($16)
lw $9,4($16)
add $10,$8,$9
sw $10,0($0)

a:
.word 123
.word 543
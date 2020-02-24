  # labwork.S
  # Written 2015-2017 by F Lundevall
  
# Skeleton file for IS1200/IS1500 lab 1.
  
# The macros PUSH and POP are in the public domain.
 
 # Please add your own code at the end of the file.

  
#
  
# Please keep the two macros PUSH and POP unchanged
  
#
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  
#
  # Please add your own code below this line
  
#

hexasc:
	addi 	$t0, $0, 10
	
	andi 	$v0, $a0, 15	
	
	slt 	$t1, $v0, $t0
	
	bne  	$t1, $0, to_asci
	nop
	
	addi 	$v0, $v0, 7
	
to_asci:
	addi 	$v0, $v0, 0x30
	
	jr 	$ra
	nop
	
	
.global delay
delay:
	
	addi	$t0, $0, 10000
while:
	beq	$a0, $0, return
	nop
	addi	$a0, $a0, -1
	addi	$t1, $0, 0
for:
	addi	$t1, $t1, 1
	bne	$t1, $t0, for
	nop
	j	while
	nop

return:
 jr $ra
 nop


.global time2string
time2string:


	PUSH 	$ra
	PUSH	$s0
	PUSH	$s1
	
	move	$s1, $a0
	move	$s0, $a1
	
	andi	$t0, $s0, 0x0f
	andi	$t1, $s0, 0x0f0
	
	slt	$t0, $0, $t0
	slt	$t1, $0, $t1
	beq	$t0, $0, ding
	nop
not_ding:
	
	srl	$a0, $s0, 12
	jal	hexasc
	nop
	sb	$v0, 0($s1)
	
	srl	$a0, $s0, 8
	jal	hexasc
	nop
	sb	$v0, 1($s1)
	
	addi	$t0, $0, 0x3a
	sb	$t0, 2($s1)
	
	srl	$a0, $s0, 4
	jal	hexasc
	nop
	sb	$v0, 3($s1)
	
	add	$a0, $s0, $0
	jal	hexasc
	nop
	sb	$v0, 4($s1)	

	addi	$t0, $0, 0x00
	sb	$t0, 5($s1)
	
ding_back:
	POP	$s1
	POP	$s0
	POP	$ra
	jr	$ra
	nop

ding:
	
	bne	$t1, $0, not_ding
	nop
	
	addi	$t0, $0, 0x44
	sb	$t0, 0($s1)
	
	addi	$t0, $0, 0x49
	sb	$t0, 1($s1)
	
	addi	$t0, $0, 0x4e
	sb	$t0, 2($s1)
	
	addi	$t0, $0, 0x47
	sb	$t0, 3($s1)
	
	addi	$t0, $0, 0x00
	sb	$t0, 4($s1)
	
	j	ding_back
	nop
	
.global enable_interrupt

enable_interrupt:
	ei
	jr $ra
	nop

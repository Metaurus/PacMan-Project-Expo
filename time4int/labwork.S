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
.data
.text

.global hexasc
.global time2string
.global delay
.global enable_interrupt

hexasc: 
	#tests if the value of $a0 is less than 10, if it is then it jumps to numbers, else it jumps to letters 
	andi $a0, $a0, 0x0f 	#Only last 4 bits matter 
	slti  $t0, $a0, 10
	beq   $t0, 1, numbers
	nop

semicolon: 
	addi $v0, $a0, 48
	jr $ra 
	nop

numbers:
	#Added requred value 0x30 in order to reach the number 1,2,3,4,5,6,7,8,9 on AScii table 
	addi $v0, $a0, 0x30
	jr $ra
	nop 

delay: 
	PUSH $v0
	PUSH $s0
	move $s0, $a0
	whileLoop:
	#$t1 checks if $a0 is less than 0
	slti $t1, $s0, 0
	
	beq $t1, 1, whileLoopDone 
	nop
	
	addi $s0, $s0, -1
	li $t0, 0
	forLoop:
	
	#checks if $t0 is less than 4711, if not, branch outside the for loop
	slti $t2, $t0, 5000

	li $v0, 32
	syscall
	nop
	
	beq $t2, 0, forLoopDone
	nop
	
	addi $t0, $t0, 1
	
	j forLoop
	nop
	
	forLoopDone:
	
	j whileLoop
	nop
		
	whileLoopDone:
	
	POP $s0
	POP $v0
	
	jr $ra
	nop

time2string:
	PUSH $s0
	PUSH $s1 
	PUSH $ra

	addi $s0, $a0, 0
	addi $s1, $a1, 0
	
	#Write first 4 set of bits - first digit
	srl $a0, $s1, 12 
	jal hexasc
	nop
	sb $v0, 0($s0)
	
	#Write second set of 4 bits - second digit 
	srl $a0, $s1, 8
	jal hexasc 
	nop
	sb $v0, 1($s0)
	
	#Write colon 
	addi $t0, $0, 0x3A 
	sb $t0, 2($s0)
	
	#Write third set of 4 bits - third digit 
	srl $a0, $s1, 4
	jal hexasc
	nop
	sb $v0, 3($s0)
	
	#Write fourth set of 4 bits - fourth^ digit 
	srl $a0, $s1, 0
	jal hexasc
	nop
	sb $v0, 4($s0)
	
	#Write null 
	addi $t0, $0, 0x00
	sb $0, 5($s0) 
	
	POP $ra 
	POP $s1
	POP $s0
	 
	jr $ra
	nop

enable_interrupt:
	ei
	jr $ra

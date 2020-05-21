#Kevin Nounga
#Version n0: 1.0
#Date: 2020, March 10
#This code below perform simple arithmetic operations which are + - * /. In case the user entered the wrong operator it ask the user to input new values
#$t0: variable 1
#$t1: operator
#$t2: variable 2
#$t3: +
#$t4: -
#$t5: *
#$t6: /
#$t7: Y
#$t8: y
#$s0: result of the operation performed
#all this register above are used to compare with the user inputs.

SYSCALL_PRTINT	= 1	# print integer value
SYSCALL_PRTSTR	= 4	# print string value
SYSCALL_GETINT  = 5	# input integer value
SYSCALL_GETCHAR  = 12	# input integer value
SYSCALL_EXIT	= 10	# Exit


.data	# Data declaration section
h1: 			.asciiz "Bye Bye, see you soon......."
n1:				.asciiz	"Enter first number: "
b1: 			.asciiz "Enter operator: "
n2:				.asciiz	"Enter second number: "
n3: 			.asciiz "Are you finished? (Y or y for yes and N or n for no): "
n4: 			.asciiz "please be careful for the operator above. Restart below and watch yourself."
msgOut:			.asciiz	"The result is: "
newLine:		.asciiz	"\n"
adding: 		.byte '+'
subtracting: 	.byte '-'
multipliying: 	.byte '*'
dividing: 		.byte '/'
ENDN: 			.byte 'Y'
ENDn: 			.byte 'y'

ENDY:           .byte 'N'
ENDy:   		.byte 'n'

.text
.globl main 

main:		# Start of code section

debut:
# Prompt for and get first input into $t0
	li $v0, SYSCALL_PRTSTR
	la $a0, n1
	syscall
	
	li $v0, SYSCALL_GETINT
	syscall
	move $t0, $v0	#NOTE- what instruction ACTUALLY executes here?  Why?

# prompt for and get operator into $t1
	li $v0, SYSCALL_PRTSTR
	la $a0, b1
	syscall
	
	li $v0, SYSCALL_GETCHAR
	syscall
	addi $t1, $v0, 0
	
	li $v0, SYSCALL_PRTSTR
	la $a0, newLine
	syscall
	
# Prompt for and get second input into $t2
	li $v0, SYSCALL_PRTSTR
	la $a0, n2
	syscall

	li $v0, SYSCALL_GETINT
	syscall
	move $t2, $v0	
		
	jal comparison #checks what operator was enter
	j prompting	   #ask the user if to re-enter input
	jr $ra		   #Exit:
	
comparison:
	lb $t3, adding			#load add operator to $t3
	lb $t4, subtracting		#load subtract operator to $t4
	lb $t5, multipliying	#load muliply operator to $t5
	lb $t6, dividing		#load division operator to $t6
	
	beq $t1, $t3, Addition 			#check if operator entered was addition
	beq $t1, $t4, Subtraction 		#check if operator entered was subtraction
	beq $t1, $t5, Multiplication 	#check if operator entered was multiplicaton
	beq $t1, $t6, Division 			#check if operator entered was Division
	
	j ErrorI 	#if wrong operator was entered, ask the user to for new input
	jr $ra		#Exit
	
#case 1: Addition
Addition:
	add $s0, $t0, $t2
	j Result
	jr $ra
	
#case 2: Subtraction
Subtraction:
	sub $s0, $t0, $t2
	j Result
	jr $ra

#case 3: Multiplication
Multiplication:
	mul $s0, $t0, $t2
	j Result
	jr $ra

#case 4: Division
Division:
	div $t0, $t2
	mflo $t2
	move $s0, $t2
	j Result
	jr $ra
	
# Display the results
Result:	
	li $v0, SYSCALL_PRTSTR
	la $a0, msgOut
	syscall
	
	li $v0, SYSCALL_PRTINT
	move $a0, $s0	
	syscall
	jr $ra
	
#if wrong operator was entered
ErrorI:
	li $v0, SYSCALL_PRTSTR
	la $a0, n4
	syscall
	
	li $v0, SYSCALL_PRTSTR
	la $a0, newLine
	syscall
	
	li $v0, SYSCALL_PRTSTR
	la $a0, newLine
	syscall
	
	j debut #prompt user for new inputs

#if the user wants to enter new input
prompting:
	li $v0, SYSCALL_PRTSTR
	la $a0, newLine
	syscall
	
	li $v0, SYSCALL_PRTSTR
	la $a0, n3
	syscall
	
	li $v0, SYSCALL_GETCHAR
	syscall
	move $t2, $v0
	
	li $v0, SYSCALL_PRTSTR
	la $a0, newLine
	syscall
	
	li $v0, SYSCALL_PRTSTR
	la $a0, newLine
	syscall
	
	lb $t7, ENDN #load $t7 with Y
	lb $t8, ENDn #load $t8 with y
	lb $t6, ENDY #load $t6 with N
	lb $t5, ENDy #load $t5 with n
	
	#compares the user input if it is Y or y
	beq $t2, $t7, Endp 
	beq $t2, $t8, Endp
	beq $t2, $t5, debut
	beq $t2, $t6, debut
	j prompting

#ends the program
Endp:
	li $v0, SYSCALL_PRTSTR
	la $a0, h1
	syscall
	li $v0, SYSCALL_EXIT
	syscall
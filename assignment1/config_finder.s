.set noreorder # No reordering of instructions by the assembler
.set noat

.globl __start # Defining the entry point of the program

.data 	     					# Data segment, we store a tuple (x/y)
.org 0x2000						# x = 0 if we have Nothing for Data Hazards/ 1 if not
	results: 					# y = 1 if we have Delay Slot for Control Hazards/ 1 if not
		.word -1, -1

# The code segment of the program starting at address 0x80020000
.text
.org 0x80020000
# Entry point of the program
__start:
	addi $s0, $zero, results	# Load the results' address into s0

DATA_HZRDS:	
	addi $t3, $t3, 1			# t3 = 1
	addi $t4, $t4, 2			# t4 = 2

	addi $t0, $t0, 1			# t0 = 1
	addi $t1, $t0, 0			# t1 should be 1 here, if not we have Nothing for Data Hazards
	sw $t1, 0($s0)		    	# Store t1's value

CONTROL_HZRDS:
	beq $zero, $zero, SKIP
	addi $t2, $t2, 1			# If we have Delay Slot, then t2 will be 1
	nop
SKIP:	nop

	sw $t2, 4($s0)				# Store t2's value

	beq $t2, $t3, EXIT			# Configuration found with Delay Slot for Control Hazards
	nop
	nop

SOB_OR_BP1_OR_BP2:
	addi $t4, $t4, 0			# Distinguish Stall or Forwarding for Data Hazards
	LOOP:						# Iterations = 2
		addi $t4, $t4, -1
		nop
		nop
		bne $t4, $zero, LOOP

EXIT:
	break 						# Exit program

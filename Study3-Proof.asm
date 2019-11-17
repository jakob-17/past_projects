# Jakob H.
# September 11, 2019
#
# Computes and prints out rows of Pascal's Triangle.
# This is the extra credit option. ***

	.data

Question:
        .asciiz "How many lines of Pascal's Triangle would you like displayed? "
        
ErrorMessage:
	.asciiz "Sorry, Pascal's Triangle computed an integer too large for the program to display. Please choose a smaller amount of lines next time."	
	
MaxNum:
	.word 2147483647 		  # the largest an integer can become
		
	.text
	
main:    	
	li    	$v0, 4
        la   	$a0, Question
        syscall               		  # print input prompt

        li    	$v0, 5
        syscall               		  # input the integer
        
        bltz	$v0, main		  # retry if input is negative
        
        add	$s0, $s0, $v0	 	  # store input size in s0

	add	$s7, $s7, $s0		  # compute the number of bytes required for s0 integers
	add	$s7, $s7, $s7		  # ^
	add	$a0, $s7, $s7		  # store total bytes in a0
	li	$v0, 9			  # allocate memory code
	syscall			          # allocate the bytes and store the address of the first byte in v0 
	add	$s1, $zero, $v0		  # store the base address in s1

		
	add 	$t0, $zero, $s0		  # set t0 to total size
	addi	$t0, $t0, -1		  # decrement for adding 1 to the last index (outside loop)
Loop0:
	sw	$zero, ($v0)		  # store zeros in every index except for last
	addi	$v0, $v0, 4		  # move forward in memory allocation
	addi	$t0, $t0, -1		  # decrement counter
	bne	$t0, $zero, Loop0	  # loop if counter is not zero
	
	addi	$t7, $zero, 1		  # add a 1 to the last index in memeory allocation
	sw	$t7, ($v0)		  # store it ^
		
	add 	$t0, $zero, $s0		  # set t0 to total size again
	add	$s4, $zero, $zero	  # get s4 space counter initalized with 0
LoopSpace:
	addi	$s4, $s4, 1
	addi	$t0, $t0, -3		  # increase spacing for every 3 integers in the input size
	bgtz	$t0, LoopSpace		  # loop again if >= 0
	
	
	add	$t9, $t9, $s0		  # used to loop s0 (input) times

Loop:	
	add	$t3, $s0, $zero		  # store size of array to be manipulated
	add	$s2, $s1, $zero		  # store base starting address to be manipulated
	jal	PrintNonZero	          # go to print function
	
	add	$t3, $s0, $zero		  # restore size of array to be manipulated
	addi	$t3, $t3, -1		  # subtract one from array size since we're adding the current and next values
	add	$s2, $s1, $zero		  # restore base starting address to be manipulated
	jal 	AddInts			  # go to add function
	
	li    	$v0, 11                   # print char instruction code
        la    	$a0, 10                   # load newline value from ascii number 10
        syscall                           # print out newline
	
	addi	$t9, $t9, -1		  # decrement loop counter
	bne	$t9, $zero, Loop	  # recurse if more layers to compute
                
        li	$v0, 10			  # terminate execution instruction code
        syscall				  # end program	 
        
        
        
PrintNonZero:	
	addi	$t3, $t3, -1		  # decrement counter
	lw	$t1, ($s2)	          # load value at current index
	
	addi	$s2, $s2, 4		  # move forward in array
	beq	$t1, $zero, PrintNonZero  # recurse if current value == 0, else...
		
	li      $v0, 1                    # print integer instruction code
        add	$a0, $t1, $zero           # load value of t1 into a0
        syscall                           # print out integer value
       
        addi	$t4, $s4, 1		  # load number of spaces into t4
        addi	$t5, $zero, 10
ReduceSpace:				  # reduce extra spacing with larger integers
	addi	$t4, $t4, -1
	div 	$t1, $t5
	mflo	$t1
	bgtz	$t1, ReduceSpace   
PrintSpace:
	addi	$t4, $t4, -1		  # decrement space counter
	li    	$v0, 11                   # print char instruction code
        la    	$a0, 32                   # load space value from ascii number 32
        syscall                           # print out space string  
        bne	$t4, $zero, PrintSpace	  # print another space if t4 != 0   
        
	bne 	$t3, $zero, PrintNonZero  # recurse if more elements to print
        
        jr	$ra		  	  # jump back to Loop

	    
	    	    	    
AddInts:
	addi	$t3, $t3, -1		  # decrement counter
	lw	$t1, ($s2)	          # load value at current index
	lw	$t8, 4($s2)		  # load value at next index
	
	lw	$t7, MaxNum		
	sub	$t7, $t7, $t1		  # subtract first value from max value
	sub 	$t7, $t7, $t8		  # subtract second value from max value
	bltz	$t7, OverflowError	  # report error and quit	
	
	add	$t1, $t1, $t8		  # add the two values together and store in t1
	sw	$t1, ($s2)		  # store new value back into current index
	
	addi	$s2, $s2, 4		  # move forward in array
	bne	$t3, $zero, AddInts	  # recurse if more elements to add
	
	jr	$ra			  # jump back to Loop

			
				
OverflowError:
 	li    	$v0, 11                   # print char instruction code
        la    	$a0, 10                   # load newline value from ascii number 10
        syscall                           # print out newline
        
        li    	$v0, 4
        la   	$a0, ErrorMessage
        syscall               		  # print error
                
        li	$v0, 10			  # terminate execution instruction code
        syscall				  # end program	 
	

#Edit: 1/12/2020 6:38 pm
################################################################################################################
.data
    InputMess1: .asciiz "\nInput lenght of Array: "
    InputMess2: .asciiz "\nInput Value: "


################################################################################################################
    OutputMess1: .asciiz "\nYour Array after sort: "
    Enter_to_endline: .asciiz " \n "



################################################################################################################
.text
    main:
        Input_Array:
        # Read lenght form I/O
        li $v0,4
        la $a0,InputMess1
        syscall

        li $v0,5
        syscall

        move $s0,$v0
        sll $s1,$v0,2
        sub $sp,$sp,$s1

        #Read Value form I/O
    Print_InputMess2:
        li $v0,4
        la $a0,InputMess2
        syscall
        add $t0,$zero,$zero #i=0
        add $t1,$zero,$zero
    For_Loop_GetValue:
        bge $t0,$s0,Exit_For_Loop_GetValue

        sll $t1,$t0,2# t1 = 4*i
        
        add $t2,$t1,$sp#t2 = address i($sp)

        li $v0,5
        syscall

        sw $v0,0($t2)#v0 wil be stored in address i($sp)

        li $v0,4
        la $a0,Enter_to_endline
        syscall

        addi $t0,$t0,1# i ++

        j For_Loop_GetValue
    Exit_For_Loop_GetValue:
        move $a1,$sp# sp: address of array -> a1: address of array
        move $a2,$s0# s0: lenght of array -> a2: lenght of array

        jal Bubble_Sort# Bubble_Sort(arr,n)
        
        li $v0,4
        la $a0,OutputMess1
        syscall

        move $s2,$zero#i=0
    For_Loop_Print:

        bge $s2,$s0,Exit_For_Loop_Print
        sll $t0,$s2,2# to  = 4*i
        add $t1,$sp,$t0

        lw $a0,0($t1)
        li $v0,1
        syscall

        addi $s2,$s2,1
        j For_Loop_Print

    Exit_For_Loop_Print:
        add $sp,$sp,$s1

        li $v0,10
        syscall











################################################################################################################

    
Bubble_Sort:
        move $t0,$zero#i=0

        move $t2,$s0#temp = s0 = n
        subi $t2,$t2,1# temp = n-1

       

        For_Loop_i:
            move $t3,$zero
            blt $t0,$t2,For_Loop_j
            j Exit_For_Loop_i

            For_Loop_j:

                sub $t4,$t2,$t0#n-1 -i
                move $t1,$zero#j=0 
                
                loop:
                    bge $t1,$t4,If_t3
                    
                    sll $t5,$t1,2# t5 = 4*j
                    add $t5,$t5,$a1# v + 4*j

                    addi $t6,$t1,1# j+1
                    sll $t6,$t6,2# 4*(j+1)
                    add $t6,$t6,$a1# v + 4*(j+1)

                    lw $s2,0($t5)#v[j]
                    lw $s3,0($t6)#v[j+1]

                    bgt $s2, $s3, If_j
                    j Return_loop
                    If_j:
                        j Swap
                        Turn_on_flag:
                            addi $t3,$zero,1
                        Return_loop:
                            addi $t1,$t1,1# j =j +1
                    j loop
                If_t3:
                    beq $t3,$zero,Exit_For_Loop_i
            addi $t0,$t0,1
            j For_Loop_i
        Exit_For_Loop_i:
            jr $ra

            
################################################################################################################
    Swap:
            sll $t5,$t1,2# t5 = 4*j
            add $t5,$t5,$a1# v + 4*j

            addi $t6,$t1,1# j+1
            sll $t6,$t6,2# 4*(j+1)
            add $t6,$t6,$a1# v + 4*(j+1)

            lw $s2,0($t5)#v[j]
            lw $s3,0($t6)#v[j+1]

            sw $s3,0($t5)
            sw $s2,0($t6)
            j Turn_on_flag







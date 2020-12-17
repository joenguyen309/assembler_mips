# ASSEMBLER FOR MIPS'ARCHITECTURE USING C++
Make Assembler for MIPS's Architecture using C++

The instruction in MIPS can covert to Machine Code(MC) :

- R type: syscall, jr, sll, move, add, addu, and, sub
- I type: addi, andi, beq, lui, ori, bne, lbu, lhu, lui, lw, sw
- J type: j, jal
- Branch: bge, bgt, ble, blt -> convert to 2 MC.

HOW TO USE:
- The direction file ASM in _Open_File_Read()
- The direction file Txt in _Open_File_Write_and_Write_to_File () 
- If you want to add another Instruction, you must write its type in Chech_if_"name_type"_type and its opcode in Covert_"name_type"_type. And edit Decode_Regsister function.

FINALLY:
- I haven't used C++ for 1 year. So i hope you guy can optimize my code.
- Thanks for reading it. 
- Have a nice day.

        AREA    mydata, DATA, READWRITE
array   DCD     1, 2, 3, 4, 5
array2  DCD     6, 7, 8, 9 ,10

        AREA    mycode, CODE, READONLY
        EXPORT  main

main    PROC
	
		; Chapter#5 Load and Store 
;		MOV r0, #0xFF	;	Move Hexadecimal Number to r0
;		MOV r0, #2_10011100	;	Move Binary Number to r0
;		MOV r0, #54	; Move Decimal Number to r0
;		
;		MVN r1, #0xFF	; Move not of 0xFF to r1
;		MOVW r1, #5	;	Move 5 to lower half word of r1 and clear top half word
;		MOVT r1, #9	;	Move 9 to top half word of r1 and top half word is unaltered
;		
;		;LDR Rt, #<immed_8> is equivalent to MOV
;		;LDR Rt, #<immed_32> is equivalent to MOV
		
		;Pseudo Instruction LDR and ADR
		; Not a real instruction but for programmers easiness
		
;		MOV r2, #1
;		MOV r3, #1
;		
;loop	ADD r1, r2, r3
;		ADR r4, loop	;	Load a memory address

		;Accessing Data in Memory
;		LDR r0, =0x20000000
;;		LDR =0, [r0]
;		
;		LDR r1, [r0]	;	Load 4 bytes Data on this memory address in r1
;;		MOV r1, #0
;		ADD r1, r1, #4
;		STR r1, [r0]	;	Store r1 in memory address in r0
;		
;		; Memory Addressing
;		
;		;Pre-Index
;		LDR r0, =0x20008010
;		
;		LDR r1, [r0, #4]	;	Pre-Index - r1 = memory[r0 + 4]
;		LDR r2, [r0], #4	;	Post-Index - r2 = memory[r0] - r0 = r0 +4
;		LDR r3, [r0, #4]!	;	Pre-Index with update - r3 = memory[r0 + 4] - r0 = r0 + 4
		
		;Load and Store Instructions
		
;		LDR Rt, [Rn, #offsett]	;	Load word of memory address Rn + offset
;		LDRB Rt, [Rn, #offsett]	;	Load byte of memory address Rn + offset
;		LDRH Rt, [Rn, #offsett]	;	Load 2 Bytes of memory address Rn + offset
;		LDRSB Rt, [Rn, #offsett]	;	Load byte of memory address Rn + offset - Signed
;		LDRSH Rt, [Rn, #offsett]	;	Load 2 Bytes of memory address Rn + offset - Signed
;		LDM	Rt, register_list	;	Load Multiple words
;		
;		STR Rt, [Rn, #offsett]	;	Store word of memory address Rn + offset
;		STRB Rt, [Rn, #offsett]	;	Store byte of memory address Rn + offset
;		STRH Rt, [Rn, #offsett]	;	Store 2 Bytes of memory address Rn + offset
;		STM	Rt, register_list	;	Store Multiple words
		
		;Accessing an  Array
;  		LDR r0, =array
;		
;		;Pre-index
;		LDR r1, [r0]
;		LDR r2, [r0, #4]
;		LDR r3, [r0, #8]
;		LDR r4, [r0, #12]
;		LDR r5, [r0, #16]
;		
;		;Post-index
;		LDR r1, [r0], #4
;		LDR r2, [r0], #4
;		LDR r3, [r0], #4
;		LDR r4, [r0], #4
;		LDR r5, [r0], #4
;		
;		;Pre-index with update
;		LDR r0, =array
;		
;		LDR r1, [r0]
;		LDR r2, [r0, #4]!
;		LDR r3, [r0, #4]!
;		LDR r4, [r0, #4]!
;		LDR r5, [r0, #4]!
;		
;		;Loading Storing Multiple Registers
;		LDR r0, =array2
;		
;		LDMIA r0, {r1,r2,r3,r4,r5}
;		
;		ADD r1, #-1
;		ADD r2, #-1
;		ADD r3, #-1
;		ADD r4, #-1
;		ADD r5, #-1
;		
;		STMIA r0, {r1,r2,r3,r4,r5}

		;Exercise
		
		LDR r0, =0x20008000
;		MOV r1, #0x79
;		STR r1, [r0]
;		
;		LDR r2, [r0]
;		REV r1, r2

		LDMIA r0, {r1,r2,r4,r5,r6}
		
		LDMIA r0!, {r1,r2,r4,r5,r6}
		
stop    B       stop
        ENDP

        ALIGN
        END
		AREA    mydata, DATA, READWRITE
result  SPACE   4
array	DCW	1, 2, 3, 4, 5

        AREA    main, CODE, READONLY
        EXPORT  __main
        ENTRY

__main  PROC
	
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
		LDR r0, =0x82000004
		
		LDR r1, [r0]
		ADD r1, r1, #4
		STR r1, [r0]
		
stop    B       stop
        ENDP

        ALIGN
        END
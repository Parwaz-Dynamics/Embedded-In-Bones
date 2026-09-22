		AREA myData, DATA, READWRITE
x		DCD	0xFFFFFFFF
array	DCD 23,5,3,8,-1,10,6,5,2,-10
size	DCD	10
			
		AREA myCode, CODE, READONLY
			EXPORT	main
			ENTRY
			
main	PROC
	
		;Structured Programming
		
		; Example 1 - Factorial
;		LDR	r0, =x
;		
;		LDR	r1, [r0]
;		MOV	r2, #1

;factorial
;		CMP	r1, #0
;		BEQ	end_factorial
;		
;		MUL r2, r1
;		SUB	r1, #1
;		
;		B	factorial
;	
;end_factorial	

;		;Example 2 - Number of 1's in 32-bit word
;		LDR r0, =x
;		LDR r1, [r0]
;		
;		MOV r2, #0
;		
;loop			
;		LSLS	r1, r1, #1
;		ADC		r2, r2, #0
;		CBZ		r1,	stop
;		
;		B		loop

;		; Example 3 - Finding the Maximum of an Array
;		
;		LDR	r0, =array	; Address of Array
;		LDR	r1, =size	; Address of Array Size
;		
;		LDR	r2, [r1]	; Load Array Size in r2
;		MOV	r3, #0		; i
;		LDR	r4, [r0]
;		
;		LDR r5, [r0]		; Maximum number
;		LDR	r6, [r0]		; Minimum number
;		
;loop	CMP	r3, r2
;		BGE	end_function	; if i >= Array Size, end the function
;		
;		LDR	r4, [r0, #4]!
;		CMP	r4, r5
;		MOVGT	r5, r4
;		CMP	r4, r6
;		MOVLT	r6, r4
;		ADD r3, #1
;		
;		B loop
;		
;end_function
		
stop 	B	stop		
		
;while_1		
;		B while_1
	
	ENDP
	END
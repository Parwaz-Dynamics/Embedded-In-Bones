	AREA FixedPointCalculations, CODE, READONLY
	ALIGN

int2fixed	PROC
	; Argument#1: Integer
	; Argument#2: Radix Place
	
	PUSH {r4-r7, LR}
	
	MOV	r4, r0
	MOV r5, r1
	
	LSL	r7, r7, r5
	
	UMULL 
	
	POP	{r4-r7, PC}
	
	ENDP
	END
	
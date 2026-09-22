		AREA	myCode, CODE, READONLY
		EXPORT strcmp
		ENTRY
	
strcmp	PROC
	
loop	LDRB	r4, [r0], #1
		LDRB	r5, [r1], #1
		
		CMP	r4, r5
		BNE	not_same
		
		CMP	r4, #0
		BEQ	same
		
		B	loop
		
not_same
		MOV	r0, #0
		B	end_func
		
same	MOV	r0, #1
		
end_func		
		BX	LR
	
		ENDP
		END
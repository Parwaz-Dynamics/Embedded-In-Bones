		AREA	myCode, CODE, READONLY
		EXPORT	swap
		ENTRY

swap	PROC
	
		PUSH	{LR}
	
		LDR	r4, [r0]
		LDR	r5, [r1]
		STR	r5, [r0]
		STR	r4, [r1]
		
		POP	{LR}
		
		BX	LR
	
		ENDP
		END
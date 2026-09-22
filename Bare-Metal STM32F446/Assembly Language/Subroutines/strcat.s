		AREA	myCode, CODE, READONLY
		EXPORT	strcat
		ENTRY
	
strcat	PROC
		PUSH {LR}

find_strend
		LDRB	r4, [r0]
		CBZ	r4, catstr
		
		ADD	r0, r0, #1
		B	find_strend
		
		
catstr
		LDRB	r4, [r1], #1
		STRB	r4, [r0], #1
		CMP	r4, #0
		BNE	catstr
		
		
		POP	{LR}
		BX	LR

		ENDP
		END
	
		AREA	myCode, CODE, READONLY
		EXPORT	gcd
		ENTRY
		
gcd		PROC
	
		PUSH	{LR}
		
		CMP	r0, r1
		MOVLT	r4, r0
		MOVLT	r0, r1
		MOVLT	r1, r4
		
l		CBZ	r1,	exit
		SDIV	r4, r0, r1
		MLS		r5, r1, r4, r0
		
		MOV	r0, r1
		MOV	r1,	r5
		B	l
		
exit	POP	{LR}
		BX	LR
	
		ENDP	
		END
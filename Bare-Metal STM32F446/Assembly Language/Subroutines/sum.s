		AREA	myCode, CODE, READONLY
		EXPORT	sum
		ENTRY 
	
sum		PROC
	
		PUSH {LR}
	
		ADD	r0, r0, r1
		
		POP	{LR}
		
		BX	LR

		ENDP
		END
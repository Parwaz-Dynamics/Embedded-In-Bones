		AREA myCode, CODE, READONLY
		EXPORT	sum6
		ENTRY
		
sum6	PROC
		
		PUSH	{r4-r7, LR}
		
		MOV		r4, r0
		ADDS	r4, r4, r1
		ADCS	r4, r4, r2
		ADCS	r4, r4, r3
		
		LDR	r5, [SP, #20]
		LDR	r6, [SP, #24]
		
		ADC		r4, r4, r5
		ADC		r4, r4, r6
		
		MOV		r0, r4
		POP		{r4-r7, LR}
		BX		LR

		ENDP
		END
	AREA	sumCode, CODE
	EXPORT	sum4
	ALIGN
		
sum4	PROC
	
	PUSH	{r4, LR}
	
	MOV	r4, r0
	ADD	r4, r4, r1
	ADD	r4, r4, r2
	ADD	r4, r4, r3
	
	MOV	r0, r4
	
	POP	{r4, PC}
	
	ENDP
	END

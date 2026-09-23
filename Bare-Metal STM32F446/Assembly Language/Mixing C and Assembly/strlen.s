	AREA stringLength, CODE
	EXPORT	my_strlen
	ALIGN
		
my_strlen	PROC
	
	PUSH	{r4-r5, LR}
	
	MOV	r5, #0
	
loop
	LDRB	r4, [r0, r5]
	CBZ	r4, end_func
	ADD	r5, r5, #1
	
	B	loop
	
end_func

	MOV	r0, r5

	POP	{r4-r5, PC}
	
	ENDP
	END
		AREA	myCode, CODE, READONLY
		EXPORT	insert_number
		ENTRY
		
insert_number	PROC
			
		PUSH	{LR}
		
		MOV	r4, r2				;	i = array size

l1		CBZ	r4, end_function	;	if i <= 0 
		LDR	r5, [r1, r4, LSL #2]			; 	data[i]
		
		CMP	r0, r5				; 	compare data[i] with r0
		BGE	enter_number		;	if data[i] <= r0
		
		ADD	r6, r1, r4, LSL	#2
		ADD	r6, r6, #4
		STR	r5, [r6]
		
		SUB	r4, r4, #1			; 	i = i - 1
		
		B l1
		
enter_number	
		
		SUB	r6, r6, #4
		STR	r0, [r6]
		
end_function
		
		POP	{LR}
		BX	LR
			
		ENDP
		END
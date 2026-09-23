	AREA	countFuncs, CODE
	IMPORT	counter
	ALIGN
		
setCount	PROC
	EXPORT	setCount
	
	LDR	r4, =counter
	STR	r0, [r4]
	BX	LR
	
	ENDP
		
getCount	PROC
	EXPORT	getCount
	
	LDR	r4, =counter
	LDR	r0, [r4]
	BX	LR
	
	ENDP		
		
increament	PROC
	EXPORT	increament		[WEAK]
	
	LDR	r4, =counter
	LDR	r5,	[r4]
	ADD	r5, r5, #1
	STR	r5,	[r4]
	
	BX	LR
	
	ENDP
		
	END
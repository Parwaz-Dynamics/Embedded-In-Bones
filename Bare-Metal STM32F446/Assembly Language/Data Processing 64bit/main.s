		AREA	myCode, CODE, READONLY
		EXPORT	main
		ENTRY
		
main	PROC
	
;		;Counting Leading Zeros
;		LDR	r0, =0x7AAAAAAA
;		LDR	r1,	=0
;		
;		CLZ	r2, r1
;		
;		CMP	r2, #32
;		CLZEQ	r3, r0
;		
;		ADDEQ	r2, r3, r2

		;64bit shift Left
		LDR	r0, =0x7AAAAAAA
		LDR	r1,	=16
		
		MOV	r2, #60
		
		MOV	r3, r0
		
		LSL	r0, r0, r2
		LSL	r1, r1, r2
		
		CMP	r2, #32
		BLE	simple
		
		SUB	r5, r2, #32
		LSL	r3, r3, r5
		ADD	r1, r1, r3
		
		B	stop
		
simple
		RSB	r4, r2, #32
		LSR	r3, r3, r4
		ADD	r1, r1, r3
		
stop	B stop	
	
		ENDP
		END
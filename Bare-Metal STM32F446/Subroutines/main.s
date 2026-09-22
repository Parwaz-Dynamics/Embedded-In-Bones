		AREA	myData, DATA, READWRITE
;x		DCD	10
;y		DCD 5
;result	DCD 0

str1	DCB	"String-1",0
str1_	SPACE	20
str2	DCB	"String-1",0
			
		AREA	myCode, CODE, READONLY
		EXPORT	main
;		IMPORT	sum
;		IMPORT	swap
;		IMPORT	gcd
;		IMPORT	strcat
		IMPORT	strcmp
		ENTRY
		
main	PROC
	
		;Subroutines
	
		;Pass by Value
;		LDR r0, =0xAAAAAAAA
;		LDR	r1, =0x55555555
;		
;		BL	sum
;		
;		LDR	r4, =result
;		STR	r0, [r4]
		
;		;Pass by Reference
;		LDR	r0, =x
;		LDR	r1,	=y
;		
;		BL	swap

;		;Example of Greatest Common Divisor
;		MOV	r0, #2310
;		MOV	r1, #483
;		
;		BL	gcd
;		
;		LDR	r4, =result
;		STR	r0,	[r4]

;		;Example of 2 concatenating strings
;		
;		LDR	r0, =str1
;		LDR	r1, =str2
;		
;		BL	strcat

		;Example of comparing 2 strings
		
		LDR	r0, =str1
		LDR	r1, =str2
		
		BL	strcmp
		
		
		
stop	B stop
	
		ENDP
		END
		
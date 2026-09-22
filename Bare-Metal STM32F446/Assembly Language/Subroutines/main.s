		AREA	myData, DATA, READWRITE
;x		DCD	10
;y		DCD 5
;result	DCD 0

;str1	DCB	"String-1",0
;str1_	SPACE	20
;str2	DCB	"String-1",0

array	DCD	1,2,4,5,6,7
			
		AREA	myCode, CODE, READONLY
		EXPORT	main
;		IMPORT	sum
;		IMPORT	swap
;		IMPORT	gcd
;		IMPORT	strcat
;		IMPORT	strcmp
;		IMPORT	insert_number
		IMPORT	sum6
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
		
;		LDR	r0, =str1
;		LDR	r1, =str2
;		
;		BL	strcmp

;		;Example of Inserting Number in Array
;		MOV	r0, #3
;		LDR	r1, =array
;		MOV	r2, #5
;		
;		BL insert_number		

;		;Pass Arguments through Stack
;		
;		MOV	r0, #1
;		MOV	r1, #2
;		MOV	r2, #3
;		MOV	r3, #4
;		
;		PUSH	{r0, r1}
;		
;		MOV	r0, #5
;		MOV	r1, #6
;		
;		BL	sum6
		
stop	B stop
	
		ENDP
		END
		
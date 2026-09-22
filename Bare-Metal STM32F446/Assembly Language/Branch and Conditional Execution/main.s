		AREA myData, DATA, READWRITE
variable 	DCD 1
msg			DCB	"Hello", 0

		AREA myCode, CODE, READONLY
		EXPORT main

main	PROC
		
		; Branch and Conditional Execution
		
		; Condition Testing
		
loop	

;		; Unsigned
;		
;		LDR r1, =0x00000001	; x
;		LDR r2, =0xFFFFFFFE	; y
;			
;		CMP r1, r2	; compare x - y
;		BLS	else	; if x <= y 
;then	MOV	r3, #1
;		B	endif	; skip next instruction
;			
;else	MOV r3, #0
;endif

;		; Signed
;		
;		LDR r1, =0x00000001	; x
;		LDR r2, =0xFFFFFFFE	; y
;			
;		CMP r1, r2	; compare x - y
;		BLE	else	; if x <= y 
;then	MOV	r3, #1
;		B	endif	; skip next instruction
;			
;else	MOV r3, #0
;endif

;		; Compare and Branch
;		LDR r1, =0x00000000	; x
;		LDR r2, =0xFFFFFFFE	; y

;		CBZ	r1, abc
;		
;		ADD r1, r2
;		
;abc		B loop

		; Conditional Execution
		
;		MOV r1, #10	; x
;		MOV r2, #10	; y
;		
;		CMP r1, r2
;		ADDEQ r3, r1, r2	;	Add if r1 and r2 are equal
;		ANDNE r3, r1, r2	;	Subtract if r1 and r2 are not equal

		; if-then Statement
		
;		MOV r0, #10
;		MOV r1, #20
		
;		;C code
;		if(a < 0)
;		{
;			a = -a;
;		}
;		x = x + 1;

		;Methode 1 
		
;		CMP r0, #0
;		BGE	endif
;then	RSB r0, #0
;			
;endif	ADD r1, r1, #1
		
		;Methode 2

;		CMP r0, #0
;		RSBLT	r0, #0
;endif	ADD r1, r1, #1

		; Compound Boolean Expression
		
		; C Code - Logical OR
;		if(x <= 20 || x >= 25){
;			a =1;
;		}

;		MOV r0, #16
		
;		CMP r0, #20
;		MOVLE	r1, #1
;		CMP r0, #25
;		MOVGE	r1, #1
		
		; C Code - Logical AND
;		if(x > 20 && x < 25){
;			a =1;
;		}

		; Logical AND in Assembly is A and B = not(A) or not(B) - De Morgan's Law
;		so not(x > 20) = x <= 20
;		   not(x < 25) = x >= 25
;		(x > 20 && x < 25) = (x <= 20 || x >= 25)

;		CMP r0, #20
;		MOVLE	r1, #1
;		CMP r0, #25
;		MOVGE	r1, #1

		; C Code - Logical AND and Logical OR
;		if(x == 5 || (x > 20 && x < 25))
;			a = 1;

		; My idea but takes more clock cycles
;		MOV r0, #21

;		CMP r0, #5
;		MOVEQ	r1, #1
;		CMP r0, #20
;		MOVLE	r1, #1
;		CMP r0, #25
;		MOVGE	r1, #1

;		CMP	r0, #5
;		BEQ then
;			
;		CMP	r0, #20
;		BLE endif
;			
;		CMP	r0, #25
;		BGE endif
;		
;then	MOV r1, #0
;endif

		; if-then-else Statement
		
		; C Code
;		if(a == 3)
;			b = 4;
;		else
;			b = 2;

;		MOV r0, #6
;		
;if		CMP r0, #3
;		BEQ	then

;else	MOV	r1, #2		
;		B	endif
;			
;then 	MOV r1, #4
;endif

		; for loop
		
;		for(initial_expression; condition_expression; loop_expression){}

		;C Code
;		int i = 0;
;		int sum = 0;
;		
;		for(i; i < 10; i++)
;		{
;			sum += i;
;		}

;		MOV r0, #0
;		MOV r1, #0
;		
;l1		CMP	r0, #10
;		BGE	exit_loop
;		
;		ADD	r1, r0
;		
;		ADD r0, #1
;		B l1
;		
;exit_loop

		; while loop
		
		; while(condition_expression){}
		;C Code
;		int i = 10;
;		int sum = 0;
;		while( i > 0 ){
;		sum += i;
;		i--;}

;		MOV r0, #10
;		MOV r1, #0
;		
;check	CMP r0, #0
;		BLE endloop
;		
;		ADD r1, r0
;		SUB	r0, #1
;		B	check
;		
;endloop	

		; do-while loop
		; C Code
;		int sum 0;
;		int i = 10;
;		do{
;		sum += i;
;		i--;
;		} while( i > 0 );

;		MOV r0, #10
;		MOV r1, #0
;		
;do		ADD r1, r0
;		SUB	r0, #1
;		CMP	r0, #0
;		BGT	do

		; continue statement
		
		;C Code
;		int i = 0
;		int sum = 0
;		for(i = 0; i < 10; i++)
;		{
;			if (i == 5 || i == 8)
;				continue;
;			sum += i;
;		}

;		MOV r0, #0
;		MOV r1, #0
;		
;check 	CMP r0, #10
;		BGE	end_loop
;		
;		ADD r0, #1
;		
;		CMP	r0, #5
;		BEQ	check
;		
;		CMP	r0, #8
;		BEQ	check
;		
;		ADD r1, r0
;		B check
;		
;end_loop

		; break statement
		
		;C Code
;		int i = 0
;		int sum = 0
;		for(i = 0; i < 10; i++)
;		{
;			if (i == 5 || i == 8)
;				continue;
;			sum += i;
;		}

;		MOV r0, #0
;		MOV r1, #0
;		
;check 	CMP r0, #10
;		BGE	end_loop
;		
;		ADD r0, #1
;		
;		CMP	r0, #5
;		BEQ	check
;		
;		CMP	r0, #8
;		BEQ	check
;		
;		ADD r1, r0
;		B check
;		
;end_loop

		; break statement
			
;		find null in a string
;		LDR r0, =msg
;		MOV r1, #0
;		
;l1		LDRB	r2, [r0]
;		
;		CMP r2, #0
;		BEQ	break
;		
;		ADD r1, #1
;		ADD r0, #1
;		B	l1
;		
;break

		;switch
		
		MOV r0, #3
		
		CMP r0, #1
		BLE	default
		
		SUBS	r2, r0, #2
		LDR r1, =switch_table
		LDR pc, [r1, r2, LSL #2] 
		
switch_table
		DCD	case_2
		DCD	case_3
		DCD	case_4
			
case_4
		MOV r7, #4
		B	end_switch
case_3
		MOV r7, #3
		B	end_switch
case_2
		MOV r7, #2
		B	end_switch
		
default
		MOV r7, #0
		
end_switch
		
		B loop

		ENDP
			
		ALIGN
		END
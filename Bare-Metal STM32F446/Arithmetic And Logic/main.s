		AREA    mydata, DATA, READWRITE
result  SPACE   4

        AREA    main, CODE, READONLY
        EXPORT  __main
        ENTRY

__main  PROC
	
		; Shift and Rotate
;        LDR     r1, =0x80000003

;        LSL     r2, r1, #1      ; r2 = 0x00000006 - Left Shift
;        LSR     r3, r1, #1      ; r3 = 0x40000001 - Right Shift
;        ASR     r4, r1, #1      ; r4 = 0xC0000001 - Arithmetic Shift
;        ROR     r5, r1, #1      ; r5 = 0xC0000001 - Left Rotate - LSB shift to MSB - LSB to Carry

;        MOVS    r7, #0          ; Clear Carry flag (C = 0)
;        RRX     r6, r1          ; r6 = 0x40000001 - Similar to ROR

		; Arithmetic 
		
		; Addition & Subtraction
;		MOVS	r1, #5
;		MOVS	r2, #3
;		
;		SUB 	r3, r1, r2	; r3 = r1 - r2
;		SBC		r3, r1, r2	; r3 = r1 - r2 + Carry - 1
;		RSB		r3, r1, r2	; r3 = r2 - r1
;		
;		SUB		r3, r1, #4	; r3 = r1 - 4
;		
;		RSB 	r3, r1, r1, LSL #5	; r3 = r1 << 2 - r1 = r1 * 31
;		ADD 	r3, r1, r1, LSL #3	; r3 = r1 + r1 << 2 = r1 * 9		
;		
;		ADDS	r3, r1, r2	; r3 = r1 + r2

;		;Book Example 
;		; A = 00001234,00000002,FFFFFFFF
;		; B = 1234567B,00000004,00000001
;		; C = A - B
;		
;		; Loading A
;		LDR	r0, =0xFFFFFFFF	; A's lower 32 bits
;		LDR r1, =0x00000002	; A's middle 32 bits
;		LDR	r2, =0x00001234	; A's upper 32 bits
;		
;		; Loading B
;		LDR	r3, =0x00000001	; B's lower 32 bits
;		LDR r4, =0x00000004	; B's middle 32 bits
;		LDR	r5, =0x12345678	; B's middle 32 bits
;		
;		; C = A - B
;		SUBS	r6, r0, r3	; C[31:0] = A[31:0] - B[31:0], update carry
;		SBCS	r7, r1, r4	; C[63:32] = A[63:32] - B[63:32] + Carry - 1, update carry
;		SBC		r8, r2, r5	; C[95:64] = A[95:64] - B[95:64] + Carry - 1

		; Short Multiplication & Division
;		MOVS r0, #0x10
;		MOVS r1, #0x2
;		MOVS r3, #1
;		
;		MUL	r2, r0, r1 ; r2 = LSB32(r0 * r1)
;;		UMUL r2, r0, r1 ; Not for ARM
;		MLA	r2, r0, r1, r3 ; r2 = LSB32(r0 * r1) + 1
;		MLS r2, r0, r1, r3 ; r2 = LSB32(r0 * r1) - 1
;		
;		UDIV r2, r0, r1 ; r2 = r0 / r1 - Unsigned
;		
;		MOVS r0, #-0x10 ; Decimal = -16
;		SDIV r2, r0, r1 ; r2 = r0 / r1 - Signed

		; Long Multiplication & Division
;		LDR r0, =0x10
;		LDR r1, =2
;		
;		UMULL r2, r3, r0, r1 ; r4:r3 = r0 * rl, r4 = MSB bits, r3 = LSB bits - Unsigned
;		SMULL r2, r3, r0, r1 ; r4:r3 = r0 * rl, r4 = MSB bits, r3 = LSB bits - Signed
;		
;		MOVS r2, #1
;		MOVS r3, #0
;		UMLAL r2, r3, r0, r1 ; r4:r3 = r4:r3 + r0 * r1 - Unsigned
;		
;		MOVS r2, #1
;		MOVS r3, #0
;		SMLAL r2, r3, r0, r1 ; r4:r3 = r4:r3 + r0 * r1 - Signed

		; Saturation
;		MOVS r0, #10
;		USAT  r1, #3, r0 ; output range: 0 <= r2 <= 2^3
;		
;		LDR r0, =0xFFFFFFF6
;		SSAT  r1, #3, r0 ; output range: -2^(3-1) <= r2 <= 2^(3-1)

		; Barrel Shifters
;		MOV r0, #0x1000
;		ADD r1, r0, r0, LSL #3 ; r1 = r0 + r0 << 3 = r0 + r0 *8
;		ADD r1, r0, r0, LSR #1 ; r1 = r0 + r0 >> 3 = r0 + r0/8 - Unsigned
;		
;		LDR r0, =0x10001000
;		ADD r1, r0, r0, ASR #1 ; r1 = r0 + r0 >> 3 = r0 + r0/8 - Signed

		; Bitwise Logic
;		LDR r0, =0xD5755755
;		LDR r1, =0xAABAAAA9
;		
;		AND r2, r0, r1
;		ORR r2, r0, r1
;		EOR r2, r0, r1
;		ORN r2, r0, r1
;		BIC r2, r0, r1 ; NOT r1 than AND with r0
;		MVN r2, r0

		; Bit Mask
;		LDR r0, =0x34 	; 0b0011_0100 - N
;		LDR r1, =(1<<5)	; 0b0010_0000 - Mask
;		
;		; In C
;		; char N = 0x34;
;		; char Mask = 1<<5;
;		
;		ANDS r2, r0, r1	; 0b0010_0000 - Symbol in C '&' - C = N & Mask - Checking the bit
;		ORR r2, r1, r0	; 0b0011_0100 - Symbol in C '|' - C = N | Mask - Setting a bit
;		BIC r2, r1, r0	; 0b0001_0100 - C = C & ~Mask - Clearing a bit
;		EOR r2, r1, r0	; 0b0001_0100 - Symbol in C '^' - C = N ^ Mask - Toggle a bit

		; Bits and Bytes Reversal
;		LDR r0, =0x12345678	; 0b0001_0010_0011_0100_0101_0110_0111_1000
;		
;		; All bit reversed
;		RBIT r1, r0 ; 		  0b0001_1110_0110_1010_0010_1100_0100_1000 - 0x1E6A2C48

;		; Reverse bytes in a word	(first 8 bits to last 8 bits and so on)
;		REV r2, r0	;		  0b0111_1000_0101_0110_0011_0100_0001_0010 - 0x78563412
;		
;		; Reverse bytes in a half word
;		REV16 r3, r0	;	  0b0011_0100_0001_0010_0111_1000_0101_0110 - 0x34127856
;		
;		; Reverse the bytes in lower half word and sign extension
;		LDR r0, =0x33448877	; 0b0011_0011_0100_0100_1000_1000_1001_1001
;		REVSH r4, r0	; 	  0b1111_1111_1111_1111_1001_1001_1000_1000 - 0xFFFF9988 - 15th bit will be extended
		
		; Sign and Zero Extension
;		LDR r0, =0x11228091
;		
;		SXTB r1, r0	; Sign extended after a byte
;		SXTH r1, r0	; Sign extended after a word
;		
;		UXTB r1, r0	; Zero extended after a byte
;		UXTH r1, r0	; Zero extented after a word

		; Data Comparison
;		MOV r0, #10
;		MOV r1, #5
;		
;		CMP r0, #3	;	Compare r0 with 3 - Carry flag 
;		CMN r0, #10	;	Compare r0 with -10 - No Carry flag
;		CMP r0, r1	; 	Compare r0 with r1 - Carry flag
;		TEQ r0,#'?'	;	Compare r0 with ASCII letter '?' - Carry flag unchanged
;		
;		MOV r1, #(1<<31)	;	r1 = 0x80000000
;		TST r0, r1	;	Compare bits of r0 and r1 - Zero flag & Carry flag unchanged
		
stop    B       stop
        ENDP

        ALIGN
        END
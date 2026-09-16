        AREA    mydata, DATA, READWRITE
result  SPACE   4

        AREA    main, CODE, READONLY
        EXPORT  __main
        ENTRY

__main  PROC
        LDR     r2, =0xF0000001     ; r1 = 0xF0000001
        MOVS    r1, #1              ; r2 = 1
        ANDS    r3, r1, r2, LSL	#1         ; r3 = r1 AND r2 = 1
        LDR     r0, =result
        STR     r3, [r0]            ; write r3 into RAM
stop    B       stop
        ENDP
        ALIGN
        END
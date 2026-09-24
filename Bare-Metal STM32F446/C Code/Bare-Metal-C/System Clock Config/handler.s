	AREA system_clock_settings, CODE
	EXPORT SystemClock_Config
	ALIGN

PROC SystemClock_Config
	
	MOV	r0, #TIM2_BASE
	
	ENDP
	END
	
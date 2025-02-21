;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; TurBoss 2025
;;
;; UART1 ez80f92
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	ASSUME ADL = 0								; set 16bit mode

	SECTION code_user
	
	PUBLIC	uart_init
	PUBLIC	_uart_init
	PUBLIC	uart_put
	PUBLIC	_uart_put
	PUBLIC	uart_get
	PUBLIC	_uart_get

	#include "ez80f92.inc"
	
	; UART1
	PORTC_DRVAL_DEF       EQU    0ffh			;The default value for Port C data register (set for Mode 2).
	PORTC_DDRVAL_DEF      EQU    0ffh			;The default value for Port C data direction register (set for Mode 2).
	PORTC_ALT0VAL_DEF     EQU    0ffh			;The default value for Port C alternate register-0 (clear interrupts).
	PORTC_ALT1VAL_DEF     EQU    000h			;The default value for Port C alternate register-1 (set for Mode 2).
	PORTC_ALT2VAL_DEF     EQU    000h			;The default value for Port C alternate register-2 (set for Mode 2).


	; baudrate divisors 115200 UART
	; 18432000 / (16*115200) = 10
	BRD_LOW_1             EQU    00Ah
	BRD_HIGH_1            EQU    000h

	; baudrate divisors 57600 UART
	; 18432000 / (16*57600) = 20
	; BRD_LOW_1           EQU    014h
	; BRD_HIGH_1          EQU    000h

	; baudrate divisors 9600 UART
	; 18432000 / (16*9600) = 120
	; BRD_LOW_1           EQU    078h
	; BRD_HIGH_1          EQU    000h
	



uart_init:
_uart_init:
								; all pins to GPIO mode 2, high impedance input
	LD A, PORTC_DRVAL_DEF
	OUT0 (PC_DR),A
	LD A, PORTC_DDRVAL_DEF
	OUT0 (PC_DDR),A
	LD A, PORTC_ALT1VAL_DEF
	OUT0 (PC_ALT1),A
	LD A, PORTC_ALT2VAL_DEF
	OUT0 (PC_ALT2),A
	
												; initialize for correct operation
												; pin 0 AND 1 to alternate function
												; set pin 3 (CTS) to high-impedance input
	IN0 A,(PC_DDR)
	OR  00001011b; set pin 0,1,3
	OUT0 (PC_DDR), A
	IN0 A,(PC_ALT1)
	AND 11110100b								; reset pin 0,1,3
	OUT0 (PC_ALT1), A
	IN0 A,(PC_ALT2)
	AND 11110111b ; reset pin 3
	OR  00000011b ; set pin 0,1
	OUT0 (PC_ALT2), A
	
	IN0 A,(UART1_LCTL)
	OR 10000000b 								; set UART_LCTL_DLAB
	OUT0 (UART1_LCTL),A
	LD A, BRD_LOW_1 							;// Load divisor low
	OUT0 (UART1_BRG_L),A
	LD A, BRD_HIGH_1 							;// Load divisor high
	OUT0 (UART1_BRG_H),A
	IN0 A,(UART1_LCTL)
	AND 01111111b								; reset UART_LCTL_DLAB
	OUT0 (UART1_LCTL),A
	LD A, 000h									; reset modem control register
	OUT0 (UART1_MCTL),A
	LD A, 007h									; enable AND clear hardware fifo's
	OUT0 (UART1_FCTL),A
	LD A, 000h									; no interrupts
	OUT0 (UART1_IER),A
	IN0 A, (UART1_LCTL)
	OR  00000011b								; 8 databits, 1 stopbit
	AND 11110111b								; no parity
	OUT0 (UART1_LCTL),A

	RET


uart_put:
_uart_put:
	PUSH	AF
@uart_avail:
	IN0		A,				(UART1_LSR)
	AND		40h									; 01000000 = Transmit holding register/FIFO and transmit shift register are empty
	JR		Z,				@uart_avail
	POP 	AF
	OUT0	(UART1_THR),	L
	RET


uart_get:
_uart_get:
;@uart_avail:
	IN0		A,				(UART1_LSR)
	BIT		0,				A					; Check if the receive buffer is full
;	JR 		Z,				@uart_avail			; immediately if no character is available
	RET		Z									; immediately if no character is available
	IN0		E,				(UART1_RBR)			; Read the character from the receive buffer
	RET




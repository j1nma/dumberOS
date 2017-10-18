GLOBAL cpuVendor
GLOBAL sti
GLOBAL setPicMaster
GLOBAL setPicSlave
GLOBAL irq0Handler
GLOBAL irq1Handler
GLOBAL irq2Handler
GLOBAL sysCallHandler
GLOBAL sysOutLong
GLOBAL sysInLong
GLOBAL sysInByte
GLOBAL sysInWord
GLOBAL sysOutWord
GLOBAL sysOutByte

EXTERN irqDispatcher
EXTERN sysCallDispacher

EXTERN switchUserToKernel
EXTERN switchKernelToUser

%include "./asm/macros.m"

section .text

; Los nombres de sysOutLong, sysOutWord, sysOutByte, sysInLong, sysOutWord y sysInByte son sacados de OSDev.
; Esta funcion habla con los perifericos.
; Se le pasa por 'rdi' el address de 16 bits y por rsi la data de 32 bits.
sysOutLong:
	push rbx
	push rax 

	mov rbx, rdi

	mov rax, rsi

	mov dx, bx
	out dx, eax
	

	pop rax
	pop rbx
	ret


; Esta funcion habla con los perifericos.
; Se le pasa por 'rdi' el address de 16 bits y por rsi la data de 16 bits.
sysOutWord:
	push rbx
	push rax 

	mov rbx, rdi

	mov rax, rsi

	mov dx, bx
	out dx, ax
	
	pop rax
	pop rbx
	ret

; Esta funcion habla con los perifericos.
; Se le pasa por 'rdi' el address de 16 bits y por rsi la data de 8 bits.
sysOutByte:
	push rbx
	push rax 

	mov rbx, rdi

	mov rax, rsi

	mov dx, bx
	out dx, al
	

	pop rax
	pop rbx
	ret

; Esta funcion habla con los perifericos.
; Se le pasa por 'rdi' el address de 16 bits y devuelve por 'eax' un valor de 32 bits.
sysInLong:
	push rbx

	mov rbx, rdi

	mov rax, rsi

	mov dx, bx
	in eax, dx

	pop rbx

	ret

; Esta funcion habla con los perifericos.
; Se le pasa por 'rdi' el address de 16 bits y devuelve por 'eax' un valor de 16 bits.
sysInWord:
	push rbx

	mov rbx, rdi
	and rbx, 0xFFFF ;16 bits

	mov rax, rsi

	mov dx, bx
	in ax, dx
	and rax, 0xffff;
	
	pop rbx

	ret


; Esta funcion habla con los perifericos.
; Se le pasa por 'rdi' el address de 16 bits y devuelve por 'eax' un valor de 8 bits.
sysInByte:
	push rbx

	mov rbx, rdi

	mov rax, rsi

	mov dx, bx
	in al, dx
	and rax, 0xff;
	
	pop rbx

	ret


irq0Handler:
	irqHandler 0

	;pushaq
	;push flags

	; save current process' RSP
	;mov rdi, rsp

	; enter kernel context by setting current process' kernel-RSP
	;call switchUserToKernel

	;mov rsp, rax

	; schedule, get new process' RSP and load it
	;call switchKernelToUser

	;mov rsp, rax

	; send end of interrupt
	;mov al, 0x20
	;out 0x20, al

	;pop flags
	;popaq
	;iretq

irq1Handler:
	irqHandler 1

irq2Handler:
	irqHandlerSlave 2


sysCallHandler:

	; El orden de las syscalls es rax, rbx, rcx y rdx.
	; El orden de las funciones en C son rdi, rsi, rdx, rcx, r8 y r9.
	; Aca solamente tengo que redistribuir los registros.

	pushaq
	;pushf ;Aca pusheo todo al stack actual.

	mov [reg_a], rax
	mov [reg_b], rbx
	mov [reg_c], rcx
	mov [reg_d], rdx ;Guardo las variables pasadas por la syscall en memoria ya que al
					 ; cambiar de stack, las puedo perder. Mas facil asi.
	
	; save current process' RSP
	mov rdi, rsp
	call switchUserToKernel
	mov rsp, rax ;pongo el puntero de switchUserToKernel en el stack pointer.

	mov rax, [reg_a]
	mov rbx, [reg_b]
	mov rcx, [reg_c]
	mov rdx, [reg_d] ;Vuelvo a levantar los registros de memoria.

	mov r8, rdx
	mov rdi, rax
	mov rsi, rbx
	mov rdx, rcx
	mov rcx, r8 ; Todo esto mapea C a asm en el handling de los parametros.

	call sysCallDispacher
	
	mov [result], rax ; Como 'popaq' pone todos los registros en su valor original, guardo 'rax' (mi valor de retorno)
					  ; en una variable en memoria para despues de llamar a 'popaq' volver a asignarlo a 'rax'.
	
	push rax
	push rax
	push rax
	push rax ; Aca pusheo muchas cosas al stack, para mostrar que cuando vuelvo al stack
			 ; del user, esta limpio y todos los registros estan bien :)

	call switchKernelToUser
	mov rsp, rax ;pongo el puntero de switchKernelToUser en el stack pointer.
	popaq

	mov rax, [result]

	iretq
	

; parametros 1ro registro
; retorna en al


sti:
	sti
	ret

setPicSlave:
	push rbp
	mov rbp, rsp
	
	mov rax, rdi
	out 0A1h, al
	
	mov rsp, rbp
	pop rbp
	ret
	
setPicMaster:
	push rbp
	mov rbp, rsp
	
	mov rax, rdi
	out 21h, al
	
	mov rsp, rbp
	pop rbp
	ret

cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid

	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

PCI_CONFIG_ADDRESS	EQU	0x0CF8
PCI_CONFIG_DATA		EQU	0x0CFC

section .data
	result dq 0
	reg_a  dq 0
	reg_b  dq 0
	reg_c  dq 0
	reg_d  dq 0



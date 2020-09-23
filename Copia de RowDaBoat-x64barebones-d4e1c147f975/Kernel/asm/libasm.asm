GLOBAL cpuVendor
GLOBAL num_to_string
GLOBAL getKey
GLOBAL segundos
GLOBAL minutos
GLOBAL horas
GLOBAL obtenerCantidad

section .bss
    numstr resb 10  ; used by num_to_string function

section .data
    amount db 0

section .text

getKey:
    push rbp
    mov rbp, rsp
    mov rax, 0

    in al, 0x64  ; si el teclado tiene algo para darme, en el bit menos significativo habra un 1
    and al, 0x01
    cmp al, 1
    je .keyFound
    jmp .fin  ; voy directamente al final, y devuelvo el valor 0 que tiene ahora rax
.keyFound:
    in al, 0x60  ; le doy el codigo de la letra
.fin:
    mov rsp, rbp
    pop rbp
    ret

segundos:
	push rbp
	mov rbp, rsp

	mov al, 0
	call obtenerCantidad

	mov rsp, rbp
	pop rbp
	ret

minutos:
	push rbp
	mov rbp, rsp

	mov al, 2
	call obtenerCantidad

	mov rsp, rbp
	pop rbp
	ret

horas:
	push rbp
	mov rbp, rsp

	mov al, 4
	call obtenerCantidad

	mov rsp, rbp
	pop rbp
	ret

obtenerCantidad:
	push rbp
	mov rbp, rsp

	out 70h, al
	in al, 71h
	mov bl, al    ; hago un backup de la cantidad
	and al, 00001111b
	mov [amount], al

	mov al, bl
	and al, 11110000b	; PERO ES UN NUMERO EN BINARIO, DEBO DIVIDIR POR 16
	shr al, 4
	mov cl, 10
	mul cl			; multiplico cl por al. El resultado se deja en al
	add [amount], al

	mov al, [amount]		;muevo la cantidad a AL para retornar
	
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

; --------------------------------
; num_to_string:  convierte un numero a una cadena con los caracteres de ese numero
; argumentos:   RDI = el numero a convertir
; retorno:     eax = cadena del numero
; ---------------------------------
num_to_string:           ;acordate que el cociente de la division lo guarda en eax, el resto en edx, y el divisor en ecx
    push rbp
    mov rbp, rsp
    push rbx
    push rcx
    push rdx

    mov rax, rdi
    mov rcx, 10             ;voy dividiendo de a 10
    mov rdx, 0              ;inicializo en 0 para preparar la division (acordate de EDX:EAX)
    mov rbx, numstr
    add rbx, 9              ;me posiciono al final del string
    mov byte [rbx], 0       ;cargo el 0 final del string
    dec rbx
.sigo:
    div rcx
    add dl, 0x30           ;al sumarle 30 paso un valor numerico a su caracter ASCII
    mov [rbx], dl          ;lleno el espacio vacio con el caracter
    dec rbx
    cmp rax, 0
    je .termino
    mov rdx, 0       ;lleno edx con cero ya que tengo que preparar la division (acordate de EDX:EAX)
    jmp .sigo
.termino:
    inc rbx           ;voy para adelante un espacio ya que ahora mismo estoy parado 1 posicion atras del numero
    mov rax, rbx
    pop rdx
    pop rcx
    pop rbx
    mov rsp, rbp
    pop rbp
    ret
GLOBAL num_to_string
;syscalls
GLOBAL syscall_write
GLOBAL syscall_read
GLOBAL syscall_malloc
GLOBAL syscall_free
GLOBAL syscall_check_mem_state
GLOBAL syscall_buddy_malloc
GLOBAL syscall_buddy_free
GLOBAL syscall_buddy_check_mem_state
GLOBAL syscall_create_process
GLOBAL syscall_halt
GLOBAL syscall_ps
GLOBAL syscall_get_pid
GLOBAL syscall_kill
GLOBAL syscall_force_new_selection
GLOBAL syscall_change_state
GLOBAL syscall_set_priority
GLOBAL syscall_block
GLOBAL syscall_unblock

section .bss
    numstr resb 10  ; used by num_to_string function 


section .text

;DSP HACERLO SOLO CON ESTA
; syscall:
;     int 80h
;     ret


;SYSCALLS PARA MANEJO DE MEMORIA
syscall_buddy_check_mem_state:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 10 

    int 80h

    pop rdx
    pop rcx
    pop rbx
    ;pop rax

    mov rsp, rbp
    pop rbp
    ret


syscall_buddy_malloc:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 8      ;ID para malloc del buddy
    mov rbx, 1      ;no se usa en realidad, TODO: chequear si lo sacamos
    mov rcx, 1      ;idem
    mov rdx, rdi    ;Le paso el size
    mov r8, 1

    int 80h

    pop rdx
    pop rcx
    pop rbx
    ;pop rax

    mov rsp, rbp
    pop rbp
    ret


syscall_buddy_free:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 9      ;ID para free del buddy
    mov rbx, 1      ;no se usa en realidad, TODO: chequear si lo sacamos
    mov rcx, 1      ;idem
    mov rdx, 1      ;idem
    mov r8, rdi     ;le paso la direccion

    int 80h

    pop rdx
    pop rcx
    pop rbx
    ;pop rax

    mov rsp, rbp
    pop rbp
    ret



syscall_free:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 6      ;ID para malloc
    mov rbx, 1      ;no se usa en realidad, TODO: chequear si lo sacamos
    mov rcx, 1      ;idem
    mov rdx, 1      ;idem
    mov r8, rdi     ;le paso la direccion

    int 80h

    

    pop rdx
    pop rcx
    pop rbx
    ;pop rax

    mov rsp, rbp
    pop rbp
    ret

syscall_check_mem_state:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 7      ;ID para malloc
    mov rbx, 1      ;no se usa en realidad, TODO: chequear si lo sacamos
    mov rcx, 1      ;idem
    mov rdx, 1      ;Le paso el size
    mov r8, rdi

    int 80h

    pop rdx
    pop rcx
    pop rbx
    ;pop rax

    mov rsp, rbp
    pop rbp
    ret


syscall_malloc:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 5      ;ID para malloc
    mov rbx, 1      ;no se usa en realidad, TODO: chequear si lo sacamos
    mov rcx, 1      ;idem
    mov rdx, rdi    ;Le paso el size
    mov r8, 1

    int 80h

    pop rdx
    pop rcx
    pop rbx
    ;pop rax

    mov rsp, rbp
    pop rbp
    ret

;;MI IDEA ES QUE LA SYSCALL RETORNE EL VALOR QUE OBTUVO.
;;sino vemos despues como hacemos

;SYSCALLS READ Y WRITE
syscall_write:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 4
    mov rbx, 1
    mov rcx, rdi ; le paso el string
    mov rdx, rsi ; le paso la longitud del string

    int 80h

    pop rdx
    pop rcx
    pop rbx
    pop rax
    mov rsp, rbp
    pop rbp
    ret


;SYSCALLS CORRESPONDIENTES AL SCHEDULER Y PROCESOS
syscall_create_process:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 13
    mov rbx, rdi
    mov r9, rcx
    mov rcx, 1
    mov r8, rdx
    mov rdx, rsi ; le paso la longitud del string


    int 80h

    pop rdx
    pop rcx
    pop rbx
    ; pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_halt:
     push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 18
    int 80h

    pop rdx
    pop rcx
    pop rbx
    pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_get_pid:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 11
    int 80h

    pop rdx
    pop rcx
    pop rbx
    mov rsp, rbp
    pop rbp
    ret

syscall_force_new_selection:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 17
    int 80h

    pop rdx
    pop rcx
    pop rbx
    pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_ps:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 14
    int 80h

    pop rdx
    pop rcx
    pop rbx
    pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_kill:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 12
    mov rbx, rdi
    int 80h

    pop rdx
    pop rcx
    pop rbx
    ; pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_change_state:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 16
    mov rbx, rdi
    int 80h

    pop rdx
    pop rcx
    pop rbx
    ; pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_block:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 19
    mov rbx, rdi
    int 80h

    pop rdx
    pop rcx
    pop rbx
    ; pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_unblock:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 20
    mov rbx, rdi
    int 80h

    pop rdx
    pop rcx
    pop rbx
    ; pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_set_priority:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 15
    mov rbx, rdi
    mov rcx, rsi
    int 80h

    pop rdx
    pop rcx
    pop rbx
    ; pop rax
    mov rsp, rbp
    pop rbp
    ret

syscall_read:
    push rbp
    mov rbp, rsp
    push rax
    push rbx
    push rcx
    push rdx

    mov rax, 3
    mov rbx, rdi  ; le paso el file_descriptor
    mov rcx, rsi  ; le paso la direccion de donde tiene que guardar lo leido
    mov rdx, rdx  ; le paso la cantidad de caracteres que tiene que leer
    mov r8, 1
    int 80h

    pop rdx
    pop rcx
    pop rbx
    pop rax
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
%define BUFFER_SIZE 32

section .data
	char_diff: db 0x00
	buf : times 256 db 0

section .text

; %rdi - address of string
; OUT:
; %rax - length of string
string_length:
  xor rax, rax
  .loop:
    cmp  byte[rdi + rax], 0
    je   .exit
    inc  rax
    jmp  .loop
  .exit:
  ret

; %rdi - address of string
print_string:
  call string_length ; Put string length into %rax
  ; Prepare registers for sys_write syscall
  mov rsi, rdi ; Source address (string address)
  mov rdi, 1 ; Output file descriptor (stdout)
  mov rdx, rax ; Amount of bytes to write
  mov rax, 1 ; sys_write syscall code
  syscall
  ret

; %rdi - code of character to print
print_char:
  push rdi ; Push character to stack
	; Prepare registers for sys_write syscall
  mov rsi, rsp ; Source address (address of peak of the stack)
  mov rdi, 1 ; Output file descriptor (stdout)
  mov rdx, 1 ; Amount of bytes to write (1 in our case)
  mov rax, 1 ; sys_write syscall code
  syscall
  pop rdi ; Pop character from stack
  ret

print_newline:
  mov rdi, 10 ; Print newline character
  jmp print_char

; %rdi - address of string
; %rsi - address of new string
string_copy:
  call string_length
  mov  rcx, rax ; Put string length into rcx
  inc  rcx ; Increment to take into account null terminator
  xchg rsi, rdi ; Swap %rsi and %rdi
  mov  byte[rdi + rax], 0 ; Put null terminator into the end of new string
  rep  movsb ; Move bytes from %rsi to %rdi
  ret

; %rdi - unsigned number to print
print_uint:
  push rbp ; Save %rbp
  mov  rbp, rsp ; Move address of peak of the stack into %rbp
  mov  rax, rdi ; Save number to print into %rax
  mov  rdi, 10 ; Put divider (10) into %rdi
  sub  rsp, BUFFER_SIZE ; Allocate buffer on stack
  dec  rbp ; Allocate place for null terminator on peak of the stack
  mov  byte[rbp], 0 ; Put null terminator on peak of the stack
  .loop:
    dec  rbp
    xor  rdx, rdx
    div  rdi ; Divide by 10 and put remainder into %rdx
    add  rdx, '0' ; Remainder + code of '0'
    mov  byte[rbp], dl ; Save digit into the stack
    test rax, rax ; if %rax == 0 then ZF = 0
    jnz  .loop ; If %rax is not zero then continue
  mov rdi, rbp ; Put address of the string with digits of number
  call print_string ; Print number
  add rsp, BUFFER_SIZE ; Deallocate buffer from stack
  pop rbp ; Restore %rbp
  ret

; %rdi - signed number to print
print_int:
  test rdi, rdi ; Set flags
  jns  .unsigned ; Jump if unsigned
	; Otherwise print '-' character
  push rdi
  mov  rdi, '-'
  call print_char
  pop  rdi
  neg  rdi ; Negotiate number
  .unsigned:
  jmp  print_uint

; %rdi - address of the first string
; %rsi - address of the second string
string_equals:
  call string_length
  mov  rcx, rax ; Save length of first string into %rcx
  xchg rdi, rsi ; Swap %rdi and %rsi (put address of the second string into %rdi)
  call string_length
  cmp  rax, rcx ; Compare strings lengths
  jne  .not_equals
  repe cmpsb ; repe - repeat while equals; cmpsb - sets flags
  jne  .not_equals
  mov rax, 1 ; return 1
  ret
  .not_equals:
    mov rax, 0 ; return 0
    ret

read_char:
	push 0
	mov rsi, rsp
	mov rdx, 1
	xor rdi, rdi
	xor rax, rax
	syscall
	pop rax
	ret

read_word:
	push r14
  push r15
  xor r14, r14
  mov r15, rsi
  dec r15

.read_first:
  push rdi
  call read_char
  pop rdi
  cmp al, ' '
  je .read_first
  cmp al, 10
  je .read_first
  cmp al, 13
  je .read_first
  cmp al, 9
  je .read_first
  test al, al
  jz .read_success

.read_next:
  mov byte [rdi + r14], al
  inc r14

  push rdi
  call read_char
  pop rdi
  cmp al, ' '
  je .read_success
  cmp al, 10
  je .read_success
  cmp al, 13
  je .read_success
  cmp al, 9
  je .read_success
  test al, al
  jz .read_success
  cmp r14, r15
  je .read_err
  jmp .read_next

.read_success:
  mov byte [rdi + r14], 0
  mov rax, rdi
  mov rdx, r14
  pop r15
  pop r14
  ret

.read_err:
  xor rax, rax
	xor rdx, rdx
  pop r15
  pop r14
  ret

; %rdi - address of the unsigned number string
; OUT:
; %rax - parsed unsigned number
; %rdx - amount of read digits
parse_uint:
  call string_length
  mov rcx, rax ; Move length of the string into %rcx
  mov rsi, rdi ; Move address of the stirng into %rsi
  xor rdx, rdx ; Clear %rdx to store amount of digits
  xor rax, rax ; Clear %rax to store number
  .pars:
    xor  rdi, rdi
    mov  dil, byte[rsi + rdx] ; Move next char into %dil

    cmp  dil, '0'
    jb   .exit
    cmp  dil, '9'
    ja   .exit

    sub  dil, '0' ; Substract code of '0' from %dil
    imul rax, 10 ; Multiply %rax by 10
    add  rax, rdi ; Add digit to the %rax
    inc  rdx ; Increment amount of read digits
    dec  rcx ; Decrement length of string and set flags
    jnz  .pars ; If there are some chars then repeat
  .exit:
  ret

; %rdi - address of the unsigned number string
; OUT:
; %rax - parsed signed number
; %rdx - amount of read digits (with '-')
parse_int:
  cmp byte[rdi], '-' ; Check for '-'
  je .minus ; If there is '-' then handle it
  jmp parse_uint ; Otherwise parse uint
  .minus:
    inc rdi ; Skip '-'
    call parse_uint
    test rdx, rdx
    jz .null ; If there's no digits after '-' then exit
    neg rax ; Otherwise negotiate number
    inc rdx ; Take minus into account
    ret
  .null:
    xor eax, eax
    ret

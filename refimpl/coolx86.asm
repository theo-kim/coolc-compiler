;;;;;;;;;;;;;;;;;;;;;;;;;
; COOL x86-32 standard library
;
; This is a port of the COOL language runtime
; library.
; The original MIPS version by Alex Aiken, 1994-1995.
; This x86-32 version by Jeff Epstein, 2020.

section .data

;;;;;;;;;;;;;;;;;;;;;;;;;;
; messages and strings

_cabort_msg: db "No match in case statement for class %s",10,0
_cabort_msg2: db "Match on void in case statement."
_term_msg: db "COOL program successfully executed",10,0
_dispatch_msg: db "%s:%d: Dispatch to void.",10,0
_abort_msg: db "Abort called from class %s",10,0
_out_string_format: db "%s",0
_out_int_format: db "%d",0
_substr_too_big_msg: db "Length too big in substr",10,0
_substr_neg_pos: db "Negative position in substr",10,0
_substr_neg_len: db "Negative length in substr",10,0
_io_error_msg: db "I/O error",10,0

; from x86_64-linux-gnu/bits/resource.h
RLIMIT_STACK equ 3
rlimit_vals:
    dd -1
    dd -1

section .bss

;;;;;;;;;;;;;;;;;;;;;;;;;;
; buffer for storing text input

STRING_BUFFER_SIZE equ 1024
string_buffer: resb STRING_BUFFER_SIZE

section .text

;;;;;;;;;;;;;;;;;;;;;;;;;;
; general utility functions

global  _NoGC_Init
_NoGC_Init:
global  _NoGC_Collect
_NoGC_Collect:
    ; no-ops
    ret

global equality_test
equality_test:
    ;compare esi and edi for equality
    ;return eax if true, ebx if false
    push ebp
    mov ebp, esp
    sub esp,8
    push ecx
    cmp esi, edi
    je .ret_true

    ;if either input is void, return false
    ;this contradicts the manual, which states
    ;that void values are equal only to other
    ;void values, but corresponds to the
    ;MIPS implementation
    cmp esi, 0
    je .ret_false
    cmp edi, 0
    je .ret_false
    mov [ebp-4], eax
    mov [ebp-8], ebx

    mov ecx, [esi]
    mov edx, [edi]
    cmp ecx, edx
    jne .ret_false ; tags don't match, we're done

    cmp edx, [_string_tag]
    je .string_cmp
    cmp edx, [_int_tag]
    je .int_cmp
    cmp edx, [_bool_tag]
    je .int_cmp

    ; pointer compare
    cmp esi, edi
    je .ret_true
    jmp .ret_false

.string_cmp:
    lea eax, [esi+16]
    push eax
    lea eax, [edi+16]
    push eax
    call strcmp
    add esp, 8
    cmp eax, 0
    je .ret_true
    jmp .ret_false
.int_cmp:
    mov ecx, [esi+12]
    mov edx, [edi+12]
    cmp ecx, edx
    je .ret_true
    jmp .ret_false

.ret_false:
    mov eax, [ebp-8]
    jmp .ret
.ret_true:
    mov eax, [ebp-4]
.ret:
    pop ecx
    mov esp, ebp
    pop ebp
    ret

global _case_abort
_case_abort:
    ;eax contains object
    mov ebx, [eax] ; get objtag
    mov ebx, [class_nameTab+ebx*4] ; get classname obj
    lea ebx, [ebx+16] ; get actual string

    push ebx
    push _cabort_msg
    call printf
    sub esp, 8

    push 1
    call exit

global _case_abort2
_case_abort2:
    ;filename is eax, line number in esi
    push esi
    add eax, 16
    push eax
    push _cabort_msg2
    call printf
    add esp,12
    push 1
    call exit


global _dispatch_abort
_dispatch_abort:
    ;filename str object in eax
    ;line number in esi
    ;no return
    push esi
    add eax, 16
    push eax
    push _dispatch_msg
    call printf
    add esp,12
    push 1
    call exit


;;;;;;;;;;;;;;;;;;;;
; String methods

global  String.substr
String.substr:
    push ebp
    mov ebp, esp
    sub esp, 8
    push ecx
    mov ecx, eax
    mov [ebp-4], ecx ; save self

    mov eax, [ebp+8]  ; new string length
    mov eax, [eax+12] ; length as int


    add eax, 21  ; acount for other stuff
    mov ebx, eax
    or ebx,  3   ; if it has non-even bytes
    jz .no_round
    and eax, 0xfffffffc
    inc eax      ; then round up
.no_round:
    mov [ebp-8], eax ; save rounded size
    push eax
    call malloc
    add esp, 4
    mov dword [eax], -1    ; 1 init gc header
    add eax, 4
    mov ecx, [ebp-4]
    mov edx, [ecx+0] ; 2 copy obj tag
    mov [eax+0], edx
    mov edx, [ebp-8]
    shr edx, 2
    mov [eax+4], edx ; copy word size 
    mov dword [eax+8], String_dispTab ; 4 vtable
    mov edx, [ebp+8]
    mov [eax+12], edx ; save intobj length

    lea edi, [eax+16]

    mov esi, [ebp-4]
    add esi, 16
    mov edx, [ebp+12] ; start pos obj
    add esi, [edx+12] ; start pos int
    mov ebx, [edx+12]
    cmp dword [edx+12], 0
    jl .error_negative_pos

    mov edx, [ebp+8]
    mov ecx, [edx+12] ; get length as int
    cmp ecx, 0
    jl .error_negative_length
    add ebx, ecx ; ebx holds last index position
    mov edx, [ebp-4]
    mov edx, [edx+12]
    mov edx, [edx+12]
    cmp ebx, edx
    jg .error_too_big

    rep movsb
    mov byte [edi], 0

    pop ecx
    mov esp, ebp
    pop ebp
    ret 8
.error_too_big:
    push _substr_too_big_msg
    jmp .abnormal_exit
.error_negative_length:
    push _substr_neg_len
    jmp .abnormal_exit
.error_negative_pos:
    push _substr_neg_pos
.abnormal_exit:
    call printf
    add esp, 4
    push 1
    call exit

global  String.length
String.length:
    mov eax, [eax+12]
    ret

global  String.concat
String.concat:
    push ebp
    mov ebp, esp
    sub esp,12
    push ecx
    mov ecx, eax

    mov eax, [ecx+12] ; str1 len
    mov edx, [eax+12] ; str1 int len
    cmp edx, 0
    je .return_arg

    mov eax, [ebp+8] ; get str2
    mov eax, [eax+12] ; str2 len
    mov eax, [eax+12] ; str2 int len
    cmp eax, 0
    je .return_self

    ; allocation of string
    ;     strlength = edx 
    ;    +gcheadec  = 4
    ;    +objtag    = 4
    ;    +objlen    = 4
    ;    +vptr      = 4
    ;    +strlen    = 4
    ;    +nul       =1

    ; allocation of int
    ;   gcheader = 4
    ;   +objtag = 4
    ;   +objlen = 4
    ;   +vptr = 4
    ;   +int=4

    add edx, eax      ; total length of new str
    mov [ebp-8], ecx  ; save self
    push edx
    mov eax, [ecx+12] ; get int obj
    call Object.copy
    pop edx
    mov [eax+12], edx  ; set new size object
    mov [ebp-4], eax   ;save the int obj

    mov eax, edx ; size to malloc
    add eax, 21  ; acount for other stuff
    mov ebx, eax
    or ebx,  3   ; if it has non-even bytes
    jz .no_round
    and eax, 0xfffffffc
    inc eax      ; then round up
.no_round:
    mov [ebp-12], eax ; save rounded size

    push eax
    call malloc
    add esp, 4

    mov dword [eax], -1    ; init gc header
    add eax, 4

    mov ecx, [ebp-8]
    mov edx, [ecx+0] ; copy obj tag
    mov [eax+0], edx
    mov edx, [ebp-12]
    shr edx, 2
    mov [eax+4], edx ; copy word size
    mov dword [eax+8], String_dispTab
    mov edx, [ebp-4]
    mov [eax+12], edx ; save intobj length

    lea edi, [eax+16]
    lea esi, [ecx+16]
    mov ecx, [ecx+12]
    mov ecx, [ecx+12]
    cld
    rep movsb

    mov ebx, [ebp+8] ; get str2
    lea esi, [ebx+16] ; str2 len
    mov ecx, [ebx+12]
    mov ecx, [ecx+12]
    cld
    rep movsb

    mov byte [edi],0  ; add final null

    jmp .return
.return_arg:
    mov eax, [ebp+8]
    jmp .return
.return_self:
    mov eax, ecx
.return:
    pop ecx
    mov esp, ebp
    pop ebp
    ret 4

;;;;;;;;;;;;;;;;;;;;
; Object methods

global  Object.type_name
Object.type_name:
    mov ebx, [eax] ; get objtag
    mov eax, [class_nameTab+ebx*4] ; get classname obj    
    ret

global  Object.abort
Object.abort:
    mov ebx, [eax] ; get objtag
    mov ebx, [class_nameTab+ebx*4] ; get classname obj
    lea ebx, [ebx+16] ; get actual string

    push ebx
    push _abort_msg
    call printf
    sub esp, 8

    push 1
    call exit

global  Object.copy
Object.copy:
    ;object to be copied in eax
    ;new copy returned in eax
    push ebp
    mov ebp, esp
    sub esp, 8
    push ecx

    mov [ebp-4], eax    ; save original object
    mov ebx, [eax+4]    ; get size in words
    shl ebx,2           ; convert to bytes
    mov [ebp-8], ebx    ; save size
    push ebx
    call malloc
    add esp,4

    push dword [ebp-8]    ; size
    push dword [ebp-4]    ; src
    push eax        ; destination
    call memcpy
    add esp,12

    pop ecx
    mov esp, ebp
    pop ebp
    ret

;;;;;;;;;;;;;;;;;;;;
; IO methods

global  IO.in_int
IO.in_int:
    push ebp
    mov ebp, esp
    push ecx


    push dword [stdin]
    push STRING_BUFFER_SIZE
    push string_buffer
    call fgets
    add esp, 12
    mov ebx, eax  ; ebx has string pointer
    cmp eax, 0
    je .io_error

    push ebx
    call atoi
    add esp,4
    push eax

    mov eax, Int_protObj
    call Object.copy
    pop ebx
    mov [eax+12], ebx

    pop ecx
    mov esp, ebp
    pop ebp
    ret
.io_error:
    push _io_error_msg
    call printf
    add esp,4
    push 1
    call exit

global  IO.in_string
IO.in_string:
    push ebp
    mov ebp, esp
    sub esp,16
    push ecx

    ;ebp-4=the string
    ;ebp-8=the length int
    ;ebp-12=the length obj
    ;ebp-16=int size rounded up to word

    push dword [stdin]
    push STRING_BUFFER_SIZE
    push string_buffer
    call fgets
    add esp, 12
    mov ebx, eax  ; ebx has string pointer
    cmp eax, 0
    je .io_error
    mov [ebp-4], ebx

    xor ecx, ecx
    not ecx
    mov edi, eax
    xor al, al
    cld
    repne scasb
    not ecx
    dec ecx  ; ecx has length of string, including null

    ;; remove trailing newline, of various flavors
.again:
    cmp byte [ebx+ecx-1], 10
    je .remove_newline
    cmp byte [ebx+ecx-1], 13
    je .remove_newline
    jmp .no_newline
.remove_newline:    
    mov byte [ebx+ecx-1], 0
    dec ecx
    jmp .again
.no_newline:
    mov [ebp-8], ecx

    push ecx
    mov eax, Int_protObj
    call Object.copy
    pop ebx
    mov [eax+12], ebx    ; eax holds length object
    mov [ebp-12], eax


    mov eax, [ebp-8]
    add eax, 20
    mov ebx, eax
    or ebx,  3   ; if it has non-even bytes
    jz .no_round
    and eax, 0xfffffffc
    inc eax      ; then round up
.no_round:
    mov [ebp-16], eax ; save rounded size
    push eax
    call malloc
    add esp,4

    mov dword [eax],-1
    add eax,4
    mov ebx,[String_protObj]
    mov [eax],ebx ; copy tag
    mov ebx,[ebp-16]
    shr ebx,2
    mov [eax+4],ebx ; word size
    mov dword [eax+8], String_dispTab ; 4 vtable
    mov ebx, [ebp-12]
    mov [eax+12], ebx ; length obj
    mov ecx, [ebp-8] ; string length, including null
    lea edi, [eax+16]
    mov esi, [ebp-4]
    rep movsb

    pop ecx
    mov esp, ebp
    pop ebp
    ret
.io_error:
    push _io_error_msg
    call printf
    add esp,4
    push 1
    call exit

global  IO.out_int
IO.out_int:
    push ebp
    mov ebp,esp
    push ecx
    push eax

    mov eax, [ebp+8] ; get int object
    mov ebx, [eax+12] ; get actual int

    push ebx
    push _out_int_format
    call printf
    add esp,8

    pop eax
    pop ecx
    pop ebp
    ret 4

global  IO.out_string
IO.out_string:
    push ebp
    mov ebp,esp
    push ecx
    push eax

    mov eax, [ebp+8] ; get string object
    lea ebx, [eax+16] ; get actual string

    push ebx
    push _out_string_format
    call printf
    add esp,8

    pop eax
    pop ecx
    pop ebp
    ret 4

;;;;;;;;;;;;;;;;;;;;;;;;;;;
; labels defined in main program
extern Main.main
extern Main_protObj
extern Main_init
extern Int_protObj
extern String_protObj
extern class_nameTab
extern String_dispTab
extern _int_tag
extern _bool_tag
extern _string_tag

;;;;;;;;;;;;;;;;;;;;;;;;;;;
; stdlib functions
extern exit
extern printf
extern malloc
extern memcpy
extern stdin
extern fgets
extern atoi
extern strcmp
extern setrlimit

;;;;;;;;;;;;;;;;;;;;;;;;;;;
; main entry point
; This function is called by the
; C standard library after it initializes
global main
main:
    call init
    mov eax, Main_protObj
    call Object.copy

    mov ecx, eax  ; store new object in self register
    call Main_init
    call Main.main

    push _term_msg
    call printf
    add esp, 4

    xor eax,eax
    ret

init:
    ; increase stack limit to enable deep recursion
    push rlimit_vals
    push RLIMIT_STACK
    call setrlimit
    add esp, 8
    ret

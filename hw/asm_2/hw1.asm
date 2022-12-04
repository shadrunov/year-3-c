; Задание 
; Дан массив из 10 слов. Найти сумму остатков от деления каждого из них на 3. Результат поместить в отдельный элемент данных.
;
; https://stackoverflow.com/a/28524951/20186980


section .data

array: DW 30, 0, 0, 0, 1, 2, 33, 33, 33, 33  ; Array of 10 Define Words  
result: DW 0x0  ; DW - 2 bytes


section .text

global main
main:
    mov ebp, esp; for correct debugging

    ; start loop over array of 10 integers
    mov ecx, 10  ; init loop counter
    mov edx, array  ; store pointer to current array item
    
modulo:
    mov ax, [edx]  ; divident
    mov bl, 3  ; divisor
    div bl  ; al = ax / bl, ah = ax % bl    
    
    
    mov ebx, [result]  ; get previous value of result
    add bl, ah  ; add new remainder
    mov [result], ebx  ; write sum to result
    
    add edx, 2  ; move pointer to next item
    loop modulo


    ; print result
    call print
    
    ; exit
    call exit
    
    
    
print:    
    ; Convert EAX to ASCII and store it onto the stack
    xor eax, eax
    mov ax, [result]  ; store result to cleared EAX
    sub esp, 16  ; reserve space on the stack
    mov ecx, 10  ; divisor = 10
    mov ebx, 16  ; stack shift pointer
    
extract_digit:
    xor edx, edx            ; Don't forget it!
    div ecx                 ; Extract the last decimal digit
    add dl, '0'             ; Convert remainder to ASCII
    sub ebx, 1
    mov [esp+ebx], dl       ; Store remainder on the stack (reverse order)
    test eax, eax           ; Until there is nothing left to divide
    jnz extract_digit

    mov eax, 4              ; SYS_WRITE
    lea ecx, [esp+ebx]      ; Pointer to the first ASCII digit, https://stackoverflow.com/a/1665570/20186980
    mov edx, 16
    sub edx, ebx            ; Count of digits
    mov ebx, 1              ; STDOUT
    int 0x80                ; Call 32-bit Linux

    add esp, 16             ; Restore the stack


exit:
    mov eax, 1              ; SYS_EXIT
    xor ebx, ebx            ; Return value
    int 0x80                ; Call 32-bit Linux

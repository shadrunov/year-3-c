.CODE

f PROC
        push rcx  ; array
        push rdx  ; n
        push r8  ; shift
        push rsi
        push rbx

        ; move arguments to adequate registers
        push rcx
        push rdx
        push r8
        pop rdx
        pop rsi
        pop rdi


        ; store last (shift) items of array to stack

        ; init for loop
        mov rcx, 0
        mov ecx, edx
        
        array_to_stack:
            ; find position (number of elements - iterations left)
            push rsi
            sub esi, ecx  ; esi = n - items left
            mov ebx, [rdi + rsi * 4]  ; ebx = array[n - items left]
            pop rsi

            push rbx  ; store element array[n - items left] to stack
            loop array_to_stack


        ; move first (n - shift) elements to the end of array

        ; init for loop
        push rsi
        sub esi, edx  ; esi = n - shift
        mov rcx, 0
        mov ecx, esi
        pop rsi

        move_elements:
            push rcx

            ; source element
            sub rcx, 1  ; items left - 1
            mov ebx, [rdi + rcx * 4]  ; array[items left - 1]

            ; destination element
            add rcx, rdx  ; rcx = shift + items left
            mov [rdi + rcx * 4], ebx

            pop rcx
            loop move_elements


        ; fill in the head of array with elements from stack

        ; init for loop
        mov rcx, 0
        mov ecx, edx

        return_from_stack:
            ; get element
            pop rbx

            ; find position (iterations left)
            push rcx
            sub ecx, 1
            mov [rdi + rcx * 4], ebx  ; ebx = array[items left - 1]
            pop rcx

            loop return_from_stack


        pop rbx
        pop rsi
        pop r8  ; shift
        pop rdx  ; n
        pop rcx  ; array

        ret

f ENDP
END
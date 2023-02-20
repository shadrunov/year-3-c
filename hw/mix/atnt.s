.text
    .global f
    .type f, @function

    f:
        pushq %rdi  # array
        pushq %rsi  # n
        pushq %rdx  # shift
        pushq %rcx
        pushq %rbx


        # store last (shift) items of array to stack

        # init for loop
        movq $0, %rcx
        movl %edx, %ecx
        
        array_to_stack:
            # find position (number of elements - iterations left)
            pushq %rsi
            sub %ecx, %esi  # esi = n - items left
            movl (%rdi, %rsi, 4), %ebx  # ebx = array[n - items left]
            popq %rsi

            pushq %rbx  # store element array[n - items left] to stack
            loop array_to_stack


        # move first (n - shift) elements to the end of array

        # init for loop
        pushq %rsi
        sub %edx, %esi  # esi = n - shift
        movq $0, %rcx
        movl %esi, %ecx
        popq %rsi

        move_elements:
            pushq %rcx

            # source element
            sub $1, %rcx  # items left - 1
            movl (%rdi, %rcx, 4), %ebx  # array[items left - 1]

            # destination element
            add %rdx, %rcx  # rcx = shift + items left
            movl %ebx, (%rdi, %rcx, 4)

            popq %rcx
            loop move_elements


        # fill in the head of array with elements from stack

        # init for loop
        movq $0, %rcx
        movl %edx, %ecx

        return_from_stack:
            # get element
            popq %rbx

            # find position (iterations left)
            pushq %rcx
            sub $1, %ecx
            movl %ebx, (%rdi, %rcx, 4)  # ebx = array[items left - 1]
            popq %rcx

            loop return_from_stack


        popq %rbx
        popq %rcx
        popq %rdx  # shift
        popq %rsi  # n
        popq %rdi  # array

        ret

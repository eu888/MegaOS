global test_user_function

test_user_function:
    ; Do something harmless in user mode
    mov rax, 42
    ; sysret  ; if you want to return, but for test, just loop or fault
    ; For testing, perhaps just infinite loop
    jmp $
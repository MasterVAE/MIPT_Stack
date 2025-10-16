PUSH 0
POPR SR6

CALL CYCLE_1
CALL CYCLE_2
CALL CYCLE_1
CALL CYCLE_2
CALL CYCLE_1
CALL CYCLE_2
CALL CYCLE_1
CALL CYCLE_2
HLT

LABEL CYCLE_1
    PUSH 0
    POPR SRY
    CALL FOR_Y

    CALL ADD_COUNTER
    DRAW

    PUSHR SR6
    PUSH 130
    JB CYCLE_1
RET

LABEL CYCLE_2
    PUSH 0
    POPR SRY
    CALL FOR_Y

    CALL SUB_COUNTER
    DRAW

    PUSHR SR6
    PUSH 0
    JAE CYCLE_2
RET

LABEL FOR_Y
    PUSH 0
    POPR SRX

    CALL FOR_X

    CALL ADD_Y

    PUSHR SRY
    PUSH 45           ; ВЫСОТА
    JB FOR_Y
RET

LABEL FOR_X
    PUSHR SRX
    PUSH 23         ; СЕРЕДИНА X
    SUB
    CALL SQUARE
    POPR SR3

    PUSHR SRY
    PUSH 23          ; СЕРЕДИНА Y
    SUB
    CALL SQUARE
    POPR SR4

    PUSHR SR3
    PUSHR SR4
    ADD
    PUSH 250    ; КВАДРАТ РАДИУСА
    JB FOR_X_TRUE 

    LABEL FOR_X_FALSE
        CALL PRINT_0
        CALL ADD_X

        PUSHR SRX
        PUSH 45          ; ШИРИНА
        JB FOR_X
RET

    LABEL FOR_X_TRUE
        PUSHR SRX
        PUSH 23         ; СЕРЕДИНА X
        JBE FOR_X_TRUE_2

        CALL LINE_1
        JBE FOR_X_TRUE_2

        CALL LINE_2
        JAE FOR_X_TRUE_2

        LABEL FOR_X_FALSE_2
            CALL PRINT_0
            CALL ADD_X

            PUSHR SRX
            PUSH 45          ; ШИРИНА
            JB FOR_X

        LABEL FOR_X_TRUE_2
            CALL PRINT_1
            CALL ADD_X

            PUSHR SRX
            PUSH 45          ;ШИРИНА
            JB FOR_X
RET

LABEL ADD_X
    PUSHR SRX
    PUSH 1
    ADD
    POPR SRX
RET

LABEL ADD_Y
    PUSHR SRY
    PUSH 1
    ADD
    POPR SRY
RET

LABEL ADD_COUNTER
    PUSHR SR6
    PUSH 4         ; ШАГ
    ADD
    POPR SR6
RET

LABEL SUB_COUNTER
    PUSHR SR6
    PUSH 4         ; ШАГ
    SUB
    POPR SR6
RET

LABEL PRINT_0
    PUSHR SRY
    PUSH 45              ; ШИРИНА  
    MUL
    PUSHR SRX
    ADD
    POPR SR1
    PUSH 95
    POPM [SR1]
RET

LABEL PRINT_1
    PUSHR SRY
    PUSH 45             ; ШИРИНА
    MUL
    PUSHR SRX
    ADD
    POPR SR1
    PUSH 35
    POPM [SR1]
RET

LABEL SQUARE
    POPR SR1
    PUSHR SR1
    PUSHR SR1
    MUL
RET

LABEL LINE_1
    PUSHR SRX
    PUSH 23         ; СЕРЕДИНА X
    SUB
    PUSHR SR6
    MUL
    PUSHR SRY
    PUSH 23         ; СЕРЕДИНА Y
    SUB
    PUSH 100
    MUL
RET

LABEL LINE_2
    PUSHR SRX
    PUSH 23         ; СЕРЕДИНА X
    SUB
    PUSHR SR6
    PUSH -1
    MUL
    MUL
    PUSHR SRY
    PUSH 23             ; СЕРЕДИНА Y
    SUB
    PUSH 100
    MUL
RET
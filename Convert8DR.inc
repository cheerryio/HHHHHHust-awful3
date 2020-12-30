; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
; File:         Convert8DR.inc
; Written by:   RuiLoureiro         @masmforum
;--------------------------------------------------------------------------
; note 1:
;       If we dont want the length we should use Convert Real  8DR
;       If we      want the length we should use Convert Float 8DR              
;
; Usage:
;
;       invoke  ConvertFloat8DR, addr QwordVar, addr String
;
;                             dd 0          <--- length           #### important ####
;   String should be:  String db x dup (?)
;
;
;   Output:
;
;        clc and EAX= 0  -> converted
;
;        stc:
;                EAX= 1  -> error 1 -> string contains the word ERROR
;
;                EAX=2   -> error 2 ->            "          "    "
;
;------------------------------------------------------------------------
;
;       invoke  ConvertReal8DR, addr QwordVar, addr String
;
;
;   String should be:  String db x dup (?)
;                 or   0                     -> use internal buffer
;
;   Output:
;
;        clc and EAX= 0  -> converted   => edx = string pointer
;
;        stc:
;                EAX= 1  -> error 1 -> string contains the word ERROR
;
;                EAX=2   -> error 2 ->            "          "    "
;
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
ConvertFloat8DR     proto   :DWORD,:DWORD
ConvertReal8DR      proto   :DWORD,:DWORD
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
$EXPONENT_REAL8_MAX   equ 308
;$DECIMALPLACES_REAL8  equ 15        ; we should use 15 or less than 15
$TRUNCATE$            equ 027fh
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
SETTRUNCATE         MACRO    oldcw, truncw
                    fstcw    oldcw
                    fwait                     
                    mov      word ptr truncw, $TRUNCATE$
                    fldcw    truncw
ENDM
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
EXAMINE1REAL8       MACRO
                    ; --------------------------
                    ;          examine
                    ; --------------------------
                    mov     edx, dword ptr [ebx+4]
                    mov     ecx, edx

                    mov     al, 20h                    
                    shl     ecx, 1                      ; remove sign to carry
                    jnc     short @F
                    mov     al, '-'
                    ;
            @@:     mov     byte ptr [edi], al
                    add     edi, 1

                    mov     eax, dword ptr [ebx+0]
                    
                    ; -----------------------
                    ; get 11 bits to exponent
                    ; -----------------------
                    and     ecx, 0FFE00000H
                    cmp     ecx, 0FFE00000H
                    je      short _isNAN
                    
                    ; ---------------------
                    ;  If exponent not 0
                    ;    it is normal
                    ; ---------------------
                    or      ecx, ecx
                    jnz     short _start
                    
                    ; ----------------------------
                    ;   It should be denormal
                    ;         or zero
                    ;-----------------------------                                        
                    or      edx, eax
                    jz      _iszeroS                    
                    ; ----------------------------
                    ;      It is denormal
                    ; ----------------------------                    
                    jnz     _start
                    
                    ; ----------------------
                    ;   exponent = 7FF
                    ; remove exponent in edx
                    ; ----------------------
        _isNAN:     and     edx, 000FFFFFH
                    or      edx, eax
                    jz      _isinfinityS            ; fraction = 0
                    jnz     _erro1S                 ; is NAN

                    ; ---------------------
                    ; start load the real8
                    ; ---------------------
        _start:     fld      qword ptr [ebx]        ; st(0) = X       
ENDM
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
LOG2BASE10_X        MACRO 
                    fld      st(0)              ; get a copy
                    fxtract			      ; st(0)=significand, st(1)=exponent base 2
                    fstp     st(0)		      ; remove significand                    
                    fldlg2			      ; load log10(2)
                    fmul                        ; st(0)= log10(2) * c ~= log10(X) 
ENDM
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
CONVERT1EDXEAX      MACRO
                    LOCAL   EAXonly
                    
                    xor	    ebx, ebx        ; number of digits
                                        
                    or      edx, edx
                    jz      short EAXonly 

                    push    edi
                    mov     ecx, 10
                    
            @@:     push    eax
                    ;
                        mov     eax, edx
                        xor     edx, edx
                        div     ecx
                        mov     edi, eax    ; save eax
                    ;
                    pop     eax
                
                    div     ecx
                    
                    ; ---------
                    ; remainder
                    ; ---------
                    or      dl, 30h
                    ;
                    ; save in the string
                    ; ------------------
                    sub     esi, 1
                    mov     byte ptr [esi], dl
                    add	    ebx, 1

                    mov     edx, edi        ; return eax
                    
                    or      edx, edx
                    jnz     short @B

                    pop     edi

                    ; ----------------
                    ; Divide EAX by 10 
                    ; ----------------
        EAXonly:    mov     edx, 3435973837                
                    mov     ecx, eax
                    mul     edx                 ; gives EDX:EAX
                    shr     edx, 3

                    mov     eax, edx
                    lea     edx, [edx+edx*4]
                    shl     edx, 1              ; 2 times

                    sub     ecx, edx
                    or      cl, 30h                    

                    sub     esi, 1
                    mov     byte ptr [esi], cl
                    add	    ebx, 1

                    or      eax, eax
                    jnz     short EAXonly
ENDM
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
EXPONENTREAL_8R MACRO
                LOCAL   one,two
                
                mov     cl, 10
                mov     bx, '+e'
                or      eax, eax
                jns     short @F
                
                mov     bx, '-e'
                neg     eax                     ; make number positive
        @@:
                ; Note: the absolute value of the size could not exceed 308
      
                div     cl              
                or      al, al
                jz      short one

                mov     dh, ah
                                
                xor     ah, ah                
                div     cl
                or      al, al
                jz      short two
                
                or      ax, 3030h               ; convert to ascii

                shl     eax, 16
                or      ax, bx
                mov     [edi], eax

                or      dh, 30h
                mov     byte ptr [edi+4], dh
                add     edi, 5
                jnz     short _finish
;............................................
        two:    mov     al, ah
                mov     ah, dh
;............................................
        one:    or      ax, 3030h
                shl     eax, 16
                or      ax, bx
                mov     [edi], eax
                add     edi, 4
ENDM
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
NEGATIVEEDXEAX2     MACRO
                    neg     eax
                    adc     edx, 0
                    neg     edx
ENDM
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
.code
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
; Input:
;           pReal8 = pointer                                dd 0    <------- for lenght
;           pRcl   --------------> buffer shoud be  _buffer db x dup (?), 0
;
ConvertFloat8DR     proc     pReal8:DWORD, pRcl:DWORD
                    LOCAL    expscale     :DWORD
                    LOCAL    exponent     :DWORD
                    LOCAL    QWinteger    :QWORD
                    LOCAL    oldcw        :WORD
                    LOCAL    truncw       :WORD
                    LOCAL    string[22]   :BYTE
                                       
                    push     ebx
                    push     esi
                    push     edi                    
                    
                    mov      edi, pRcl     
                    mov      ebx, pReal8                    

                    ; --------------------------
                    ;          examine
                    ; --------------------------
                    EXAMINE1REAL8
                                                                  
                    ;----------------------------------------
                    ; the exponent is the integer of log10(X)
                    ;----------------------------------------
                    LOG2BASE10_X
                    SETTRUNCATE  oldcw, truncw                    
                    fistp    exponent           
                    fldcw    oldcw

                    ; -------------------------
                    ; Get exponent scale factor
                    ; -------------------------
                    mov     eax, $EXPONENT_REAL8_MAX                    
                    mov     edx, $DECIMALPLACES_REAL8
                    sub     edx, exponent
                    mov     expscale, edx

                    mov     ebx, offset _PowerTblDQP38
        
                    or      edx, edx
                    jns     short _cmpexpscale                     

                    neg     edx
                    mov     ebx, offset _PowerTblDQM38
                    
                    cmp     edx, 46
                    jbe     short _load0

                    ; ------------------------------------------------
                    ;               compute 10^expscale
                    ; ------------------------------------------------
    _power10:       fild    expscale
                    fldl2t             ; log2(10)
                    fmul               ; log2(10)*expscale
                    fld     st
                    frndint            ; get the characteristic
                    fxch
                    fsub    st,st(1)   ; get only the mantissa but keep the characteristic
                    f2xm1              ; 2^(mantissa)-1
                    fld1
                    fadd               ; add 1 and pop
                    fscale
                    fstp    st(1)      ; remove the characteristic
                    jmp     _multiply

    _cmpexpscale:   cmp     edx, 46
                    ja      short _power10
            
        _load0:     fld     qword ptr [ebx+edx*8]                    
                    
                    ; -------------------------                    
                    ; Multiply X by 10^expscale
                    ; -------------------------
    _multiply:      fmul
                    ; -----------------
                    ; store the integer
                    ; -----------------
                    fistp   QWinteger
                    fstsw   ax
                    fwait
                    shr     ax,1
                    jc      _erro2S  

                    ; -------------------------
                    ; convert integer to string
                    ; -------------------------
                    lea     esi, QWinteger
                    mov     eax, dword ptr [esi+0]
                    mov     edx, dword ptr [esi+4]                    

                    or      edx, edx
                    jns     short @F

                    NEGATIVEEDXEAX2
                    
            @@:     lea     esi, string+20

                    ; ----------------------------------------
                    ; Divide EAX by 10 and get the digit in CL
                    ; ----------------------------------------
                    CONVERT1EDXEAX
                    
                    mov     edx, $DECIMALPLACES_REAL8

                    ; --------------------------
                    ;   Correct the exponent
                    ; --------------------------
                    mov     eax, ebx
                    sub     eax, expscale
                    sub     eax, 1
                    ;mov     exponent, eax

                    ; ------------------------------------------
                    ; Regular notation or scientific notation ?
                    ; ebx = total number of digits in the buffer
                    ; edx = number of decimal places
                    ; ecx = exponent
                    ; ------------------------------------------
                    or      eax, eax
                    jns     _ispositive       ; exponent is positive

                    mov     ecx, eax
                    neg     ecx

                    ;cmp     ecx, 5  
                    cmp     ecx, edx
                    ja      _isscientific

                    ; --------------------------
                    ; Regular notation - decimal
                    ; --------------------------
                    mov     word ptr [edi], '.0'
                    add     edi, 2
                    
                    ; ------------------------------
                    ; If exponent = -1 it is 0.ddddd
                    ; If exponent = -2 it is 0.0dddd
                    ;         and so on
                    ; ------------------------------
                    sub     ecx, 1
                    jz      _movedecimal
                    
                    sub     edx, ecx
                    
                    ; -------------------------
                    ;        insert 0
                    ; -------------------------
            @@:     mov     byte ptr [edi], '0'
                    add     edi, 1
                    sub     ecx, 1
                    jnz     short @B

    _movedecimal:   movzx   eax, byte ptr [esi]
                    mov     byte ptr [edi], al
                    add     edi, 1
                    add     esi, 1

                    sub     edx, 1
                    jnz     short _movedecimal
                    jz      short _removeD
                    ; ------------------
                    ; remove last zeroes
                    ; ------------------
            @@:     sub     edi, 1
        _removeD:   movzx   eax, word ptr [edi-2]
                    cmp     al, '.'
                    je      _finish
                    ;
                    cmp     ah, '0'
                    je      short @B
                    jmp     _finish
;-----------------------------------------------------                                        
                    ; -------------------
                    ;   exponent >= 0
                    ; -------------------
    _ispositive:    cmp     eax, edx
                    jae     _isscientific
                
                    ; --------------------------
                    ; Regular notation - integer
                    ; --------------------------
                    add     eax, 1              ; number of integer digits
                    sub     ebx, eax            ; number of decimal digits
                    ;
            @@:     movzx   edx, byte ptr [esi]
                    mov     byte ptr [edi], dl
                    add     edi, 1
                    add     esi, 1                    

                    sub     eax, 1
                    jnz     short @B
                    ;
                    or      ebx, ebx
                    jz      _finish
                    
                    mov     byte ptr [edi], '.'
                    add     edi, 1
                    ;
            @@:     movzx   edx, byte ptr [esi]
                    mov     byte ptr [edi], dl
                    add     edi, 1
                    add     esi, 1                    

                    sub     ebx, 1
                    jnz     short @B
                    jmp     short _removeI
                    ; ------------------
                    ; remove last zeroes
                    ; ------------------
            @@:     sub     edi, 1
        _removeI:   movzx   eax, word ptr [edi-2]
                    cmp     al, '.'
                    je      _finish
                    ;
                    cmp     ah, '0'
                    je      short @B
                    jmp     _finish                                        
            ;--------------------------------------------------
            ;                 is Scientific
            ;
            ; ebx = total number of digits
            ; eax = exponent
            ;--------------------------------------------------
    _isscientific:  sub     ebx,1
                    ;
                    movzx   edx, byte ptr [esi]
                    mov     dh, '.'                    
                    add     esi, 1
                    ;
                    mov     word ptr [edi], dx
                    add     edi, 2
                    
            @@:     movzx   edx, byte ptr [esi]
                    mov     byte ptr [edi], dl
                    ;
                    add     edi, 1
                    add     esi, 1

                    sub     ebx, 1
                    jnz     short @B
                    jz      short _removeE
                    
                    ; ------------------
                    ; remove last zeroes
                    ; ------------------
            @@:     sub     edi, 1
        _removeE:   movzx   edx, word ptr [edi-2]
                    cmp     dl, '.'
                    je      short @F
                    ;
                    cmp     dh, '0'
                    je      short @B

            @@:     EXPONENTREAL_8R
                                        
        _finish:    mov      eax, edi
                    mov      edi, pRcl
                    sub      eax, edi

        _exit0:     mov      dword ptr [edi-4], eax
                    mov      byte ptr [edi+eax], 0
                                              
                    xor      eax, eax
                    clc
        _exit:      pop      edi
                    pop      esi
                    pop      ebx
                    ret
;-----------------------------------------------------------
    _isinfinityS:   sub      edi, 1
                    cmp      byte ptr [edi], '-'
                    je       short @F
                    mov      byte ptr [edi], '+'     
            @@:     mov      dword ptr [edi+1], "IFNI"
                    mov      dword ptr [edi+5], "YTIN"
                    mov      eax, 9
                    jmp      _exit0
                   
                    ;--------------------------
                    ; value to be converted = 0
                    ;--------------------------
        _iszeroS:   sub      edi, 1
                    mov      word ptr [edi], 3020h
                    mov      eax, 2
                    jmp      _exit0
                    
                    ; --------------------
                    ; is indefinite or NAN
                    ; --------------------
        _erro1S:    mov      eax, 1
                    ; -------------------
                    ; Remove sign / space
                    ; -------------------
        _errorS:    sub      edi, 1
                    mov      dword ptr [edi+0], "ORRE"
                    mov      byte ptr [edi+4], "R"
                    ;
                    mov      dword ptr [edi-4], 5
                    mov      byte ptr [edi+5], 0
                    stc
                    jmp      _exit
                                        
                    ; -----------------
                    ; invalid operation
                    ; -----------------
        _erro2S:    fclex
                    mov      eax, 2
                    jmp      _errorS
ConvertFloat8DR     endp
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?
ConvertReal8DR      proc     pReal8:DWORD, pStr:DWORD
                    LOCAL    expscale     :DWORD
                    LOCAL    exponent     :DWORD
                    LOCAL    QWinteger    :QWORD
                    LOCAL    oldcw        :WORD
                    LOCAL    truncw       :WORD
                    LOCAL    string[22]   :BYTE
                    LOCAL    pString      :DWORD
                                       
                    push     ebx
                    push     esi
                    push     edi                    
                    
                    mov      ebx, pReal8                    
                    mov      edi, pStr
                    or       edi, edi
                    jnz      short @F
                    ;
                    mov      edi, offset _Real8Buffer
                    ;
            @@:     mov      pString, edi     

                    ; --------------------------
                    ;          examine
                    ; --------------------------
                    EXAMINE1REAL8
                                                                  
                    ;----------------------------------------
                    ; the exponent is the integer of log10(X)
                    ;----------------------------------------
                    LOG2BASE10_X
                    SETTRUNCATE  oldcw, truncw                    
                    fistp    exponent           
                    fldcw    oldcw

                    ; -------------------------
                    ; Get exponent scale factor
                    ; -------------------------
                    mov     eax, $EXPONENT_REAL8_MAX                    
                    mov     edx, $DECIMALPLACES_REAL8
                    sub     edx, exponent
                    mov     expscale, edx

                    mov     ebx, offset _PowerTblDQP38
        
                    or      edx, edx
                    jns     short _cmpexpscale                     

                    neg     edx
                    mov     ebx, offset _PowerTblDQM38
                    
                    cmp     edx, 46
                    jbe     short _load0

                    ; ------------------------------------------------
                    ;               compute 10^expscale
                    ; ------------------------------------------------
    _power10:       fild    expscale
                    fldl2t             ; log2(10)
                    fmul               ; log2(10)*expscale
                    fld     st
                    frndint            ; get the characteristic
                    fxch
                    fsub    st,st(1)   ; get only the mantissa but keep the characteristic
                    f2xm1              ; 2^(mantissa)-1
                    fld1
                    fadd               ; add 1 and pop
                    fscale
                    fstp    st(1)      ; remove the characteristic
                    jmp     _multiply

    _cmpexpscale:   cmp     edx, 46
                    ja      short _power10
            
        _load0:     fld     qword ptr [ebx+edx*8]                    
                    
                    ; -------------------------                    
                    ; Multiply X by 10^expscale
                    ; -------------------------
    _multiply:      fmul
                    ; -----------------
                    ; store the integer
                    ; -----------------
                    fistp   QWinteger
                    fstsw   ax
                    fwait
                    shr     ax,1
                    jc      _erro2S  

                    ; -------------------------
                    ; convert integer to string
                    ; -------------------------
                    lea     esi, QWinteger
                    mov     eax, dword ptr [esi+0]
                    mov     edx, dword ptr [esi+4]                    

                    or      edx, edx
                    jns     short @F

                    NEGATIVEEDXEAX2
                    
            @@:     lea     esi, string+20

                    ; ----------------------------------------
                    ; Divide EAX by 10 and get the digit in CL
                    ; ----------------------------------------
                    CONVERT1EDXEAX
                    
                    mov     edx, $DECIMALPLACES_REAL8

                    ; --------------------------
                    ;   Correct the exponent
                    ; --------------------------
                    mov     eax, ebx
                    sub     eax, expscale
                    sub     eax, 1
                    ;mov     exponent, eax

                    ; ------------------------------------------
                    ; Regular notation or scientific notation ?
                    ; ebx = total number of digits in the buffer
                    ; edx = number of decimal places
                    ; ecx = exponent
                    ; ------------------------------------------
                    or      eax, eax
                    jns     _ispositive       ; exponent is positive

                    mov     ecx, eax
                    neg     ecx

                    ;cmp     ecx, 5  
                    cmp     ecx, edx
                    ja      _isscientific

                    ; --------------------------
                    ; Regular notation - decimal
                    ; --------------------------
                    mov     word ptr [edi], '.0'
                    add     edi, 2
                    
                    ; ------------------------------
                    ; If exponent = -1 it is 0.ddddd
                    ; If exponent = -2 it is 0.0dddd
                    ;         and so on
                    ; ------------------------------
                    sub     ecx, 1
                    jz      _movedecimal
                    
                    sub     edx, ecx
                    
                    ; -------------------------
                    ;        insert 0
                    ; -------------------------
            @@:     mov     byte ptr [edi], '0'
                    add     edi, 1
                    sub     ecx, 1
                    jnz     short @B

    _movedecimal:   movzx   eax, byte ptr [esi]
                    mov     byte ptr [edi], al
                    add     edi, 1
                    add     esi, 1

                    sub     edx, 1
                    jnz     short _movedecimal
                    jz      short _removeD
                    ; ------------------
                    ; remove last zeroes
                    ; ------------------
            @@:     sub     edi, 1
        _removeD:   movzx   eax, word ptr [edi-2]
                    cmp     al, '.'
                    je      _finish
                    ;
                    cmp     ah, '0'
                    je      short @B
                    jmp     _finish
;-----------------------------------------------------                                        
                    ; -------------------
                    ;   exponent >= 0
                    ; -------------------
    _ispositive:    cmp     eax, edx
                    jae     _isscientific
                
                    ; --------------------------
                    ; Regular notation - integer
                    ; --------------------------
                    add     eax, 1              ; number of integer digits
                    sub     ebx, eax            ; number of decimal digits
                    ;
            @@:     movzx   edx, byte ptr [esi]
                    mov     byte ptr [edi], dl
                    add     edi, 1
                    add     esi, 1                    

                    sub     eax, 1
                    jnz     short @B
                    ;
                    or      ebx, ebx
                    jz      _finish
                    
                    mov     byte ptr [edi], '.'
                    add     edi, 1
                    ;
            @@:     movzx   edx, byte ptr [esi]
                    mov     byte ptr [edi], dl
                    add     edi, 1
                    add     esi, 1                    

                    sub     ebx, 1
                    jnz     short @B
                    jmp     short _removeI
                    ; ------------------
                    ; remove last zeroes
                    ; ------------------
            @@:     sub     edi, 1
        _removeI:   movzx   eax, word ptr [edi-2]
                    cmp     al, '.'
                    je      _finish
                    ;
                    cmp     ah, '0'
                    je      short @B
                    jmp     _finish
                                        
            ;--------------------------------------------------
            ;                 is Scientific
            ;
            ; ebx = total number of digits
            ; eax = exponent
            ;--------------------------------------------------
    _isscientific:  sub     ebx,1
                    ;
                    movzx   edx, byte ptr [esi]
                    mov     dh, '.'                    
                    add     esi, 1
                    ;
                    mov     word ptr [edi], dx
                    add     edi, 2
                    
            @@:     movzx   edx, byte ptr [esi]
                    mov     byte ptr [edi], dl
                    ;
                    add     edi, 1
                    add     esi, 1

                    sub     ebx, 1
                    jnz     short @B
                    jz      short _removeE
                    
                    ; ------------------
                    ; remove last zeroes
                    ; ------------------
            @@:     sub     edi, 1
        _removeE:   movzx   edx, word ptr [edi-2]
                    cmp     dl, '.'
                    je      short @F
                    ;
                    cmp     dh, '0'
                    je      short @B

            @@:     EXPONENTREAL_8R
                                        
        _finish:    mov      eax, edi
                    mov      edi, pString
                    sub      eax, edi

        _exit0:     ;mov      dword ptr [edi-4], eax
                    mov      byte ptr [edi+eax], 0
                                              
                    xor      eax, eax
                    clc
        _exit:      mov      edx, edi
                    ;                    
                    pop      edi
                    pop      esi
                    pop      ebx
                    ret
;-----------------------------------------------------------
    _isinfinityS:   sub      edi, 1
                    cmp      byte ptr [edi], '-'
                    je       short @F
                    mov      byte ptr [edi], '+'     
            @@:     mov      dword ptr [edi+1], "IFNI"
                    mov      dword ptr [edi+5], "YTIN"
                    mov      eax, 9
                    jmp      _exit0
                   
                    ;--------------------------
                    ; value to be converted = 0
                    ;--------------------------
        _iszeroS:   sub      edi, 1
                    mov      word ptr [edi], 3020h
                    mov      eax, 2
                    jmp      _exit0
                    
                    ; --------------------
                    ; is indefinite or NAN
                    ; --------------------
        _erro1S:    mov      eax, 1
                    ; -------------------
                    ; Remove sign / space
                    ; -------------------
        _errorS:    sub      edi, 1
                    mov      dword ptr [edi+0], "ORRE"
                    mov      byte ptr [edi+4], "R"
                    ;
                    mov      dword ptr [edi-4], 5
                    mov      byte ptr [edi+5], 0
                    stc
                    jmp      _exit
                                        
                    ; -----------------
                    ; invalid operation
                    ; -----------------
        _erro2S:    fclex
                    mov      eax, 2
                    jmp      _errorS
ConvertReal8DR      endp
; いいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいいい?

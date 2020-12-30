.386
.model flat,stdcall
.stack 4096

option nokeyword:<name>

include \Irvine32\Irvine32.inc

.data
    include struct.asm
    str1Len dword ?
    str2Len dword ?
    buffer byte 20 dup(0)
    antiDebugInfoFormat byte "嘻嘻，你看看%08x是不是有不对呢！！！",0
.code

strLength_ proc,
    str3:ptr byte

    invoke Str_length,str3
    ret
strLength_ endp

;字符串以0结尾，获取长度
getStringLen proc uses eax esi,
    str1:ptr byte,
    putLen:ptr dword
    mov esi,str1
    mov eax,0
    COMPARESTRINGLOOP:
    cmp byte ptr[esi+eax],0
    je FINISH
    inc eax
    jmp COMPARESTRINGLOOP
    FINISH:
    mov esi,putLen
    mov dword ptr[esi],eax
    ret
getStringLen endp

;比较str1,str2(以0结尾的字符串)是否相同,返回值eax 0-false,1-true
stringEqual proc,
    str1:ptr byte,
    str2:ptr byte
    push esi
    push edi
    push ecx
    invoke getStringLen,str1,addr str1Len
    invoke getStringLen,str2,addr str2Len
    mov eax,str1Len
    cmp eax,str2Len
    jne STRINGISNOTEQUAL
    mov esi,str1
    mov edi,str2
    mov ecx,eax
    cld
    repe cmpsb
    jne STRINGISNOTEQUAL
    STRINGISEQUAL:
    mov eax,1
    jmp STRINGEQUALEND
    STRINGISNOTEQUAL:
    mov eax,0
    STRINGEQUALEND:
    pop ecx
    pop edi
    pop esi
    ret
stringEqual endp

copyBytes proc uses ecx esi edi,
    source:ptr byte,
    target:ptr byte,
    count:dword
    mov ecx,count
    mov esi,source
    mov edi,target
    rep movsb
    ret
copyBytes endp

atoi proc uses esi ecx edx,
    source:ptr byte
    mov esi,source
    invoke Str_length,esi
    mov ecx,eax     ;ecx拿到a长度
    mov eax,0
    ATOILOOP:
    movzx edx, byte ptr[esi] ; get a character
    inc esi
    cmp edx, "0"
    jb ATOIFAIL
    cmp edx, "9"
    ja ATOIFAIL
    sub edx, "0"
    imul eax, 10 ; multiply "result so far" by ten
    add eax, edx ; add in current digit
    loop ATOILOOP
    ret
    ATOIFAIL:
    mov eax,-1
    ret
atoi endp

concat proc uses eax esi edi,
    str1:ptr byte,
    str2:ptr byte
    mov esi,str1
    mov edi,str2
    invoke Str_length,str1
    add esi,eax
    invoke Str_copy,edi,esi
    ret
concat endp

numbToStr proc uses ebx ecx,
    x:dword,
    strBuffer:ptr byte

    mov     ecx,strBuffer
    mov     eax,x
    mov     ebx,10
    add     ecx,ebx             ; ecx = buffer + max size of string
    mov byte ptr[ecx],0
    dec ecx
    convertLoop:
    xor     edx,edx
    div     ebx
    add     edx,48              ; convert the digit to ASCII
    mov     BYTE PTR [ecx],dl   ; store the character in the buffer
    dec     ecx                 ; decrement ecx pointing the buffer
    test    eax,eax             ; check if the quotient is 0
    jnz     convertLoop

    inc     ecx
    mov     eax,ecx             ; eax points the string in the buffer

    ret

numbToStr ENDP

end
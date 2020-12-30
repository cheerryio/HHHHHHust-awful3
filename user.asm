.386
.model flat,stdcall
.stack 4096



option nokeyword:<name>

include \Irvine32\Irvine32.inc
stringEqual proto,str1:ptr byte,str2:ptr byte
atoi proto,source:ptr byte
decryptPassword proto,source:ptr byte,des:ptr byte
concat proto,str1:ptr byte,str2:ptr byte

.data
	include struct.asm
    buffer byte 20 dup(0)
.const
	infoOrderGoodNoLogin byte "no login when order good",0
    infoOrderGoodNoGood byte "you havent chosen any good",0
    infoOrderGoodNoStoreage byte "no good anymore",0
    infoOrderSuccess byte "success order good and remove good from cargo",0
    infoCheckAuthFail byte "check auth fail",0
    infoCheckGoodChosenFail byte "havent chosen any good when try modify",0
    seperateSymbol byte " -> ",0
    iam byte "我是cherrooooooooooo你看我是谁！",0
.code

;检查Login时name输入格式正确，0-false,1-true, 放在eax
nameFormat proc,
    nameLen:dword
    cmp nameLen,0
    jz L1
    mov eax,1
    jmp L2
    L1:
       mov eax,0
    L2:
       ret
nameFormat endp

;检查密码格式是否正确，0-false,1-true,放在eax
passwordFormat proc,
    passwordLen:dword
    cmp passwordLen,0
    jz L1
    mov eax,1
    jmp L2
    L1:
       mov eax,0
    L2:
       ret
passwordFormat endp

loginCheck proc uses edi ecx,
    inputNamePtr:ptr byte,
    inputPasswordPtr:ptr byte,
    customersPtr:ptr user_s
    mov edi,customersPtr
    assume edi:ptr user_s
    mov ecx,2       ;不能写死，需要更正
    LOGINCHECKLOOP:
    LOGINCOMPARENAME:
    invoke stringEqual,inputNamePtr,addr [edi].name  ;比较结果保存在eax中
    cmp eax,1
    je LOGINCOMPAREPASSWORD
    ;这个user不相等，切换到下一个
    jmp LOGINCHECKROUNDFAIL
    LOGINCOMPAREPASSWORD:
    cmp (user_s ptr[edi]).auth,1
    je ISOWNERLOGIN
    invoke Str_copy,addr [edi].password,addr buffer
    jmp LOGINDOCOMPAREPASSWORD
    ;对boss的密码进行解密
    ISOWNERLOGIN:
    invoke decryptPassword,addr [edi].password,addr buffer

    LOGINDOCOMPAREPASSWORD:
    invoke stringEqual,inputPasswordPtr,addr buffer
    cmp eax,1
    je LOGINCHECKSUCCESS
    LOGINCHECKROUNDFAIL:
    cmp ecx,1
    je LOGINCHECKFAIL
    add edi,sizeof user_s
    loop LOGINCHECKLOOP

    LOGINCHECKSUCCESS:
    mov eax,edi
    ret
    LOGINCHECKFAIL:
    mov eax,0
    ret
loginCheck endp

;下单成功 1-true,下单失败 0-false
;需要检查登录状态，chosenGood是否为空,商品是否还有库存
orderGood proc uses esi edi edx,
    loginUserPtr:ptr byte
    mov esi,loginUserPtr
    cmp (user_s ptr[esi]).islogin,0
    je ORDERGOODFAILNOLOGIN
    mov edi,(user_s ptr[esi]).chosenGood
    cmp edi,0
    je ORDERGOODNOGOOD
    mov eax,(good_s ptr[edi]).inTotal
    cmp eax,(good_s ptr[edi]).sellTotal
    jbe ORDERGOODNOSTORAGE
    inc (good_s ptr[edi]).sellTotal
    mov (user_s ptr[esi]).chosenGood,0
    mov edx,offset infoOrderSuccess
    call writeString
    call Crlf
    ret

  ORDERGOODFAILNOLOGIN:
    mov edx,offset infoOrderGoodNoLogin
    jmp ORDERGOODFAIL
  ORDERGOODNOGOOD:
    mov edx,offset infoOrderGoodNoGood
    jmp ORDERGOODFAIL
  ORDERGOODNOSTORAGE:
    mov edx,offset infoOrderGoodNoStoreage
    jmp ORDERGOODFAIL
  ORDERGOODFAIL:
    call writeString
    call Crlf
    mov eax,0
    ret
orderGood endp

modifySingleGoodInfo macro memberName
local NEXT,NEXTWITHOUTCHANGE,AGAIN
    push edx
    mov eax,(good_s ptr[esi]).memberName
    call writeInt
    mov edx,offset seperateSymbol
    call writeString
    mov edx,offset buffer
    call readString
    cmp eax,0
    je NEXTWITHOUTCHANGE
    mov byte ptr[edx+eax],0
    invoke atoi,edx     ;转换后的int放在eax
    cmp eax,-1
    je AGAIN
    jmp NEXT
    AGAIN:
    pop edx
    jmp PROCESSLOOP
    NEXTWITHOUTCHANGE:
    pop edx
    inc edx
    jmp PROCESSLOOP
    NEXT:
    mov (good_s ptr[esi]).memberName,eax
    pop edx
    inc edx
    jmp PROCESSLOOP
endm

modifyGoodInfo proc uses esi eax edx ecx ebx,
    loginUserPtr:ptr byte
    mov esi,loginUserPtr
    cmp (user_s ptr[esi]).auth,1
    jne MODIFYGOODINFOCHECKAUTHFAIL
    mov esi,(user_s ptr[esi]).chosenGood
    cmp esi,0
    je MODIFYGOODINFOCHECKGOODCHOSEN
    mov ecx,sizeof buffer
    mov edx,1
    PROCESSLOOP:
    cmp edx,0
    je MODIFYNAME
    cmp edx,1
    je MODIFYDISCOUNT
    cmp edx,2
    je MODIFYINPRICE
    cmp edx,3
    je MODIFYSELLPRICE
    cmp edx,4
    je MODIFYINTOTAL
    cmp edx,5
    je MODIFYSELLTOTAL

    jmp MODIFYGOODINFOEND

    MODIFYNAME:

    MODIFYDISCOUNT:
    modifySingleGoodInfo discount
    MODIFYINPRICE:
    modifySingleGoodInfo inPrice
    MODIFYSELLPRICE:
    modifySingleGoodInfo sellPrice
    MODIFYINTOTAL:
    modifySingleGoodInfo inTotal
    MODIFYSELLTOTAL:
    modifySingleGoodInfo sellTotal
  MODIFYGOODINFOCHECKAUTHFAIL:
    mov edx,offset infoCheckAuthFail
    call writeString
    call Crlf
    jmp MODIFYGOODINFOEND
  MODIFYGOODINFOCHECKGOODCHOSEN:
    mov edx,offset infoCheckGoodChosenFail
    call writeString
    call Crlf
  MODIFYGOODINFOEND:
    ret

modifyGoodInfo endp

decryptPassword proc uses eax ecx esi edi,
    source:ptr byte,
    des:ptr byte
    invoke Str_length,source
    mov ecx,eax
    mov esi,source
    mov edi,des
    mov byte ptr[esi+eax],0
    DESCRYPT:
    movzx eax,byte ptr[esi]
    xor eax,"C"
    mov byte ptr[edi],al
    inc edi
    inc esi
    loop DESCRYPT
    mov edx,des
    ret
decryptPassword endp

userNameInfo proc uses esi,
    userPtr:ptr user_s
    mov esi,userPtr
    assume esi:ptr user_s
    mov eax,offset iam
    ret
userNameInfo endp

end

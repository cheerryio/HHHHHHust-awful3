.386
.model flat,stdcall
.stack 4096

option nokeyword:<name>

include \Irvine32\Irvine32.inc
stringEqual proto,str1:ptr byte,str2:ptr byte
descryptNumber proto,number:dword
copyBytes proto,source:ptr byte,target:ptr byte,count:dword

.data
	include struct.asm
    buffer byte 20 dup(0)
	infoDisplayGoodInfoStart byte "start display info:",0
    infoName byte "name is:",0
    infoDiscount byte "discount is:",0
    infoinPrice byte "inPrice is:",0
    infoSellPrice byte "sellPrice is:",0
    infoinTotal byte "inTotal is:",0
    infoSellTotal byte "sell total is:",0
    infoRecommendIndex byte "recommend index is:",0
.code

goodNameFormat proc,
    goodNameLen:dword
    cmp goodNameLen,0
    jz L1
    mov eax,1
    jmp L2
    L1:
       mov eax,0
    L2:
       ret
goodNameFormat endp

searchGood proc,
    goodNamePtr:ptr byte,
    goodsPtr:ptr byte
    push ecx
    push edi
    mov edi,goodsPtr
    mov ecx,2       ;不能写死，需要更正
    SEACHGOODLOOP:
    GOODNAMECOMPARELOOP:
    invoke stringEqual,goodNamePtr,addr (good_s ptr[edi]).name  ;比较结果保存在eax中
    cmp eax,1
    je FINDGOODSUCCESS
    ;这个user不相等，切换到下一个
    cmp ecx,1
    je FINDGOODFAIL
    add edi,sizeof good_s
    loop GOODNAMECOMPARELOOP

    FINDGOODSUCCESS:
    mov eax,edi
    pop edi
    pop ecx
    ret
    FINDGOODFAIL:
    mov eax,0
    pop edi
    pop ecx
    ret
searchGood endp

displayGoodInfo proc,
    goodPtr:ptr byte
    push eax
    push edx
    push esi
    mov esi,goodPtr
    assume esi:ptr good_s
    mov edx,offset infoDisplayGoodInfoStart
    call writeString
    call Crlf
    mov edx,offset infoName
    call writeString
    lea edx,[esi].name
    call writeString
    call Crlf
    mov edx,offset infoDiscount
    call writeString
    mov eax,0
    mov eax,[esi].discount
    call Writeint
    call Crlf
    mov edx,offset infoinPrice
    call writeString


    ;inPrice数字需要与 'C' 异或解密,解密后的结果放在eax
    invoke descryptNumber,[esi].inPrice
    call Writeint
    call Crlf

    mov edx,offset infoSellPrice
    call writeString
    mov eax,[esi].sellPrice
    call Writeint
    call Crlf
    mov edx,offset infoinTotal
    call writeString
    mov eax,[esi].inTotal
    call Writeint
    call Crlf
    mov edx,offset infoSellTotal
    call writeString
    mov eax,[esi].sellTotal
    call Writeint
    call Crlf
    fld [esi].recommendIndex
    mov edx,offset infoRecommendIndex
    call writeString
    call writeFloat
    call Crlf
    fstp [esi].recommendIndex

    assume esi:nothing
    pop esi
    pop edx
    pop eax
    ret
displayGoodInfo endp

;（进货价/实际销售价格+已售数量/（2*进货数量））*128
;推荐度放在  good.recommendIndex中
countRecommendIndex proc uses eax esi,
    goodPtr:ptr byte
    local tempConstant:dword,tempDouble:real8
    mov esi,goodPtr
    assume esi:ptr good_s
    invoke descryptNumber,[esi].inPrice
    mov tempConstant,eax
    fild tempConstant
    fidiv [esi].sellPrice
    fild [esi].inTotal
    mov tempConstant,2
    fimul tempConstant
    fstp tempDouble
    fild [esi].sellTotal
    fdiv tempDouble
    fadd
    mov tempConstant,128
    fimul tempConstant
    fstp [esi].recommendIndex
    assume esi:nothing
    ret
countRecommendIndex endp

countAllRecommendIndex proc uses ecx eax esi,
    goodsPtr:ptr byte,
    goodsCount:dword
    mov ecx,goodsCount
    mov esi,goodsPtr
  COUNTRECOMMENDINDEXLOOP:
    invoke countRecommendIndex,esi
    cmp ecx,1
    je COUNTALLRECOMMENDGOODEND
    add esi,sizeof good_s
    loop COUNTRECOMMENDINDEXLOOP
  COUNTALLRECOMMENDGOODEND:
    ret
countAllRecommendIndex endp

;将解密后的数字放在eax里面
descryptNumber proc,
    number:dword
    mov eax,number
    xor eax,"C"
    ret
descryptNumber endp

;
rangeGoods proc uses esi edi,
    goodPtr:ptr good_s,
    rangeGoodBuffer:ptr byte,
    goodNum:dword
    local @temp[10]:real8,
          @tempIndex[10]:byte,
          @tempByte1:byte,
          @tempByte2:byte,
          @tempReal8:real8
    mov esi,goodPtr
    mov edi,rangeGoodBuffer
    mov ecx,goodNum
    mov eax,0
    assume esi:ptr good_s
    INITRANGE:
    mov @tempIndex[eax],al
    fld [esi].recommendIndex
    fstp @temp[eax*8]
    add esi,sizeof good_s
    inc eax
    loop INITRANGE
    ;冒泡排序，希望顺序从大到小
    mov ecx,goodNum
    dec ecx
    BUBBLESORT1:
    mov eax,0
    mov edx,ecx
    BUBBLESORT2:
    fld @temp[eax*8]
    fld @temp[eax*8+8]
    fcompp
    push eax
    fnstsw ax
    sahf
    pop eax
    jbe ROUNDDONE
    ;浮点数交换位置
    fld @temp[eax*8]
    fld @temp[eax*8+8]
    fstp @temp[eax*8]
    fstp @temp[eax*8+8]
    ;index交换位置
    mov bl,@tempIndex[eax]
    mov bh,@tempIndex[eax+1]
    mov @tempIndex[eax+1],bl
    mov @tempIndex[eax],bh
    ROUNDDONE:
    inc eax
    cmp eax,edx
    jne BUBBLESORT2
    loop BUBBLESORT1
    mov ecx,goodNum
    invoke copyBytes,addr @tempIndex,rangeGoodBuffer,ecx
    ret

rangeGoods endp


end

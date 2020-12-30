.386
.model flat,stdcall

.stack 4096

option nokeyword:<name>

include \Irvine32\Irvine32.inc
includelib \Irvine32\Irvine32.lib


nameFormat proto,nameLen:dword
passwordFormat proto,passwordLen:dword
loginCheck proto,inputNamePtr:ptr byte,inputPasswordPtr:ptr byte,customersPtr:ptr byte
stringEqual proto,str1:ptr byte,str2:ptr byte
goodNameFormat proto,goodNameLen:dword
searchGood proto,goodNamePtr:ptr byte,goodsPtr:ptr byte
displayGoodInfo proto,good:ptr byte
countRecommendIndex proto,goodPtr:ptr byte
countAllRecommendIndex proto,goodsPtr:ptr byte,goodsCount:dword
orderGood proto,loginUserPtr:ptr byte
copyBytes proto,source:ptr byte,target:ptr byte,count:dword
modifyGoodInfo proto,loginUserPtr:ptr byte
atoi proto,source:ptr byte
antiDebug proto,_lpExceptionRecord:ptr EXCEPTION_RECORD,_lpSEH:ptr SEH,_lpContext:ptr CONTEXT,_lpDispatcherContext:ptr DISPATCHER_CONTEXT
isDebugInfo proto
userNameInfo proto,userPtr:ptr user_s
rangeGoods proto,goodPtr:ptr good_s,rangeGoodBuffer:ptr byte,goodNum:dword

include macros.asm

.data
    include struct.asm
    include data.asm
    noDebugLoc dword ?
.const
    include constants.asm

.code

storeMenuInit proc
    finit
    mov store.goodsPtr,offset storeGoods
    mov store.goodsCount,lengthof storeGoods
    mov store.customersPtr,offset storeCustomers
    mov store.customersCount,lengthof storeCustomers
    invoke Str_copy,addr encryBossPassword,addr store.owner.password
    mov edi,store.customersPtr
    assume edi:ptr user_s
    invoke Str_copy,addr encryBossPassword,addr [edi].password
    assume edi:nothing
    mov edx,offset infoMenuInstructions
    call writeString
    call Crlf
    ret
storeMenuInit endp

userLoginMain proc
    GETNAME:
        mov edx,offset infoInputName
        call writeString
        call Crlf
        mov edx,offset inputNameBuffer
        mov ecx,sizeof inputNameBuffer
        call Readstring
        mov edi,edx
        mov byte ptr[edi+eax],0
        invoke nameFormat,eax
        cmp eax,0   ;0 nameFormat not pass
        jne GETPASSWORD
        ;nameFormat error handle
        mov loginUser.auth,0
        mov edx,offset infoInputNameError
        call writeString
        call Crlf
        ret
    GETPASSWORD:
         mov edx,offset infoInputPassword
         call writeString
         call Crlf
        mov edx,offset inputPasswordBuffer
        mov ecx,sizeof inputPasswordBuffer
        call Readstring
        mov edi,edx
        mov byte ptr[edi+eax],0
        invoke passwordFormat,eax
        cmp eax,0
        jne LOGINCHECK_L
        ;wrong passwordFormat error handle
        mov loginUser.islogin,0
        mov edx,offset infoInputPasswordError
        call writeString
        call Crlf
        ret

    LOGINCHECK_L:
        invoke loginCheck,addr inputNameBuffer,addr inputPasswordBuffer,store.customersPtr
        cmp eax,0
        je LOGINCHECKFAIL_L
        invoke copyBytes,eax,addr loginUser,sizeof user_s
        mov loginUser.islogin,1
        mov edx,offset infoLoginSuccess
        call writeString
        call Crlf
        ret
    LOGINCHECKFAIL_L:
        mov edx,offset infoLoginFail
        call writeString
        call crlf
        ret
userLoginMain endp

searchGoodMain proc
        mov edx,offset infoInputGoodName
        call writeString
        call Crlf
        mov edx,offset inputBuffer
        mov ecx,sizeof inputBuffer
        call Readstring
        mov edi,edx
        mov byte ptr[edi+eax],0
        invoke goodNameFormat,eax
        cmp eax,0
        jne FINDGOOD
        mov edx,offset infoGoodNameError
        call writeString
        call Crlf
        FINDGOOD:
        invoke searchGood,edx,store.goodsPtr
        cmp eax,0
        jne FINDGOODSUCCESS
        mov edx,offset infoDontFindGood
        call writeString
        call Crlf
        ret
        FINDGOODSUCCESS:
        mov loginUser.chosenGood,eax
        invoke displayGoodInfo,eax  ;展示good信息
        mov edx,offset infoFindGood
        call Crlf
        call writeString
        call Crlf
        ret
searchGoodMain endp

userOrderMain proc
        invoke orderGood,addr loginUser
        cmp eax,1
        jne ORDEREND
        invoke countAllRecommendIndex,store.goodsPtr,store.goodsCount
    ORDEREND:
        ret
userOrderMain endp

countRecommendIndexMain  proc
        invoke countAllRecommendIndex,store.goodsPtr,store.goodsCount
        mov edx,offset infoUpdateRecommendIndex
        call writeString
        call Crlf
        ret
countRecommendIndexMain endp

modifyGoodInfoMain proc
        invoke modifyGoodInfo,addr loginUser
        invoke countAllRecommendIndex,store.goodsPtr,store.goodsCount
        ret
modifyGoodInfoMain endp

storeMenu proc
    
    invoke storeMenuInit

    assume fs:nothing
    push offset antiDebug
    push fs:[0]
    mov fs:[0],esp
    mov noDebugLoc,offset PASSANTIDEBUG
    db 204

    NOPASSANTIDEBUG:
    pop fs:[0]
    pop eax
    invoke isDebugInfo
    invoke ExitProcess,0

    PASSANTIDEBUG:
    MENU:
        pop fs:[0]
        pop eax
        mov edx,offset infoInputChoice
        call writeString
        call Crlf
        call Readint
        cmp al,0
        jz LOGOUT
        cmp al,1
        jz LOGIN
        cmp al,2
        jz SEARCHGOODS
        cmp al,3
        jz ORDER
        cmp al,4
        jz COUNTRECOMMENDINDEX_L
        cmp al,6
        jz MODIFYGOODINFO_L
        cmp al,7
        jz FUNCTEST
        cmp al,8
        jz SHOWCS

		jmp MENU	;不是已知选项，重新选择
    LOGIN:
        invoke userLoginMain
        jmp MENU
    SEARCHGOODS:
        invoke searchGoodMain
        jmp MENU
    ORDER:
        invoke userOrderMain
        jmp MENU
    COUNTRECOMMENDINDEX_L:
        invoke countRecommendIndexMain
        jmp MENU
    SHOWCS:
        call dumpRegs
        jmp MENU
    MODIFYGOODINFO_L:
        invoke modifyGoodInfoMain
        jmp MENU
    LOGOUT:
        jmp MENU
    FUNCTEST:
        invoke rangeGoods,store.goodsPtr,addr rangeGoodBuffer,store.goodsCount
        mov esi,offset rangeGoodBuffer
        movzx eax,byte ptr[esi]
        call dumpRegs
        mov esi,offset rangeGoodBuffer
        movzx eax,byte ptr[esi+1]
        call dumpRegs
        jmp MENU
storeMenu endp

returnMainMiddle proc
    mov eax,noDebugLoc
    ret
returnMainMiddle endp




end ;storeMenu  ;去掉storeMenu几个字并在storeWIndow.asm的同样这个位置加上'storeWindow'可展示窗口
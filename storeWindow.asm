
.386
.model flat,stdcall
.stack 4096

option nokeyword:<name>

.XCREF
.nolist
include    \masm32\include\masm32rt.inc
include    myStore.inc
include    Convert8DR.inc
include    PowerTable.inc

concat proto,str1:ptr byte,str2:ptr byte
userNameInfo proto,userPtr:ptr user_s
countAllRecommendIndex proto,goodsPtr:ptr byte,goodsCount:dword
strLength_ proto,str3:ptr byte
rangeGoods proto,goodPtr:ptr good_s,rangeGoodBuffer:ptr byte,goodNum:dword
numbToStr proto,x:DWORD,strBuffer:DWORD


.LIST


MAIN_WIDTH  EQU 700
MAIN_HEIGHT EQU 500

$DECIMALPLACES_REAL8  equ 9

.data
    include struct.asm
    include data.asm
    include constants.asm
icc INITCOMMONCONTROLSEX <sizeof INITCOMMONCONTROLSEX,0>
wc  WNDCLASSEX <sizeof WNDCLASSEX,NULL,WndProc,0,0,?,?,?,COLOR_SCROLLBAR+1,NULL,szClassName,?>

szAppName       db "商城系统",0
szClassName     db "SmallMenuWindowClass"
szNullString    db 0

szCtxt1         db ' Context 1',0
szCtxt2         db ' Context 2',0
hitPoint        POINT <>
myTest1          db "myTest1",0,20 dup(0)
myTest2 db "myTest2",0
buffer db 20 dup(0)
scene dword 0

goodNameText db "商品名称",0
discountText db "折扣",0
inPriceText db "进货价",0
sellPriceText db "销售价",0
inTotalText db "进货总数",0
sellTotalText db "已售数量",0
recommendIndexText db "推荐度",0

;***********************************************************************************************

.data?
ALIGN   4

hwndMain        HWND  ?
hmenuMain       HMENU ?
hmenuContext    HMENU ?
hmenuCtxtPopup  HMENU ?
hStatusBar      HWND  ?
dwStbHeight     dd    ?
dwMinHeight     dd    ?

msg             MSG   <>

;###############################################################################################

.code

showGoodsInfo proc,hWnd:HWND
    LOCAL @hdc:HDC,
          @ps:PAINTSTRUCT,
          @rect:RECT,
          @step:POINT,
          @start:POINT,
          @current:POINT,
          @rangeGoodBuffer[10]:byte,
          @index:byte,
          @goodIndex:byte,
          @intergerBuffer[10]:byte,
          @doubleBuffer[10]:byte
    mov @step.x,100
    mov @step.y,50
    mov @start.x,10
    mov @start.y,10
    invoke BeginPaint,hWnd, ADDR @ps
    mov @hdc,eax
    push @start.x
    push @start.y
    pop @current.y
    pop @current.x

eachTitle macro member
    invoke strLength_,addr member
    invoke TextOut,@hdc,@current.x,@current.y,addr member,eax
    mov eax,@step.x
    add @current.x,eax
endm

    TITLELINE:
    eachTitle goodNameText
    eachTitle discountText
    eachTitle inPriceText
    eachTitle sellPriceText
    eachTitle inTotalText
    eachTitle sellTotalText
    eachTitle recommendIndexText

    getRange:
    invoke rangeGoods,store.goodsPtr,addr @rangeGoodBuffer,store.goodsCount

    mov ecx,store.goodsCount
    mov @index,0
    showGoodInfoByRange:
    push @start.x
    pop @current.x
    mov eax,@step.y
    add @current.y,eax
    movzx eax,@index
    movzx eax,@rangeGoodBuffer[eax]
    mov edi,store.goodsPtr
    mov ebx,sizeof good_s
    mul bl
    add edi,eax
    assume edi:ptr good_s
    lea esi,[edi].name
    invoke strLength_,esi
    push ecx
    invoke TextOut,@hdc,@current.x,@current.y,esi,eax       ;商品名称text
    pop ecx
    mov eax,@step.x
    add @current.x,eax

eachGoodColumnInfo macro member
    invoke numbToStr,[edi].member,addr @intergerBuffer        ;商品折扣
    mov ebx,eax
    invoke strLength_,ebx
    push ecx
    invoke TextOut,@hdc,@current.x,@current.y,ebx,eax
    pop ecx
    mov eax,@step.x
    add @current.x,eax
endm

    eachGoodColumnInfo discount
    eachGoodColumnInfo inPrice
    eachGoodColumnInfo sellPrice
    eachGoodColumnInfo inTotal
    eachGoodColumnInfo sellTotal

    ;输出推荐度
    push ecx
    invoke ConvertReal8DR, addr [edi].recommendIndex, addr @doubleBuffer
    pop ecx
    push ecx
    invoke TextOut,@hdc,@current.x,@current.y,addr @doubleBuffer,5
    pop ecx

    assume edi:nothing
    inc @index
    dec ecx
    cmp ecx,0
    je PAINTEND
    jmp showGoodInfoByRange

    PAINTEND:
    invoke EndPaint,hWnd, ADDR @ps
    ret
showGoodsInfo endp

WndProc PROC    hWnd:HWND,uMsg:UINT,wParam:WPARAM,lParam:LPARAM

    mov     eax,uMsg
    .if eax==WM_SIZE
        .if !(wParam&(NOT SIZE_MAXIMIZED))
            movzx   eax,word ptr lParam[0]
            mov     ecx,dwStbHeight
            movzx   edx,word ptr lParam[2]
            sub     eax,ecx
            INVOKE  MoveWindow,hStatusBar,0,eax,edx,ecx,TRUE
        .endif
        xor     eax,eax                                               ;return 0

    .elseif eax==WM_GETMINMAXINFO
        mov     edx,lParam
        mov     ecx,dwMinHeight
        mov     [edx].MINMAXINFO.ptMinTrackSize.y,ecx
        xor     eax,eax                                               ;return 0

    .elseif eax==WM_CONTEXTMENU
        movzx   ecx,word ptr lParam[0]
        movzx   edx,word ptr lParam[2]
        xor     eax,eax
        INVOKE  TrackPopupMenu,hmenuCtxtPopup,
                TPM_LEFTALIGN or TPM_TOPALIGN or TPM_NOANIMATION,
                ecx,edx,eax,hWnd,eax
        xor     eax,eax                                               ;return 0
    .elseif eax==WM_PAINT
        print str$(eax)
        cmp scene,0
        je DRAWEND
        invoke showGoodsInfo,hWnd
        DRAWEND:
            invoke ValidateRect,hWnd,NULL
            ret
    .elseif eax==WM_COMMAND
        mov     ecx,wParam
        .if ecx==IDM_EXIT           ;展示 ‘我是谁谁’ 退出
            invoke userNameInfo,store.customersPtr  ;字符串内容放在eax中
            invoke MessageBox,NULL,eax,NULL,MB_OK
            mov edx,0
        .elseif ecx==IDM_RECOMMENDATION
            invoke countAllRecommendIndex,store.goodsPtr,store.goodsCount
            mov edx,offset infoUpdateRecommendIndex
        .elseif ecx==IDM_LISTSORT
                inc scene
                invoke InvalidateRect,hWnd,NULL,FALSE
                mov edx,offset infoUpdateRecommendIndex
        .elseif ecx==IDM_CTXT_1
            mov     edx,offset szCtxt1
        .elseif ecx==IDM_CTXT_2
            mov     edx,offset szCtxt2
        .else
            xor     edx,edx
        .endif
        .if edx
            INVOKE  SendMessage,hStatusBar,SB_SETTEXT,0,edx
        .endif
        xor     eax,eax                                               ;return 0
    .elseif eax==WM_CREATE
        INVOKE  CreateStatusWindow,WS_CHILD or WS_VISIBLE,offset szNullString,hWnd,CID_STATUSBAR
        mov     hStatusBar,eax
        sub     esp,sizeof RECT
        INVOKE  GetWindowRect,eax,esp
        pop     ecx
        pop     edx
        pop     eax
        pop     ecx
        sub     ecx,edx
        mov     dwStbHeight,ecx
        push    ecx
        INVOKE  GetSystemMetrics,SM_CYCAPTION
        push    eax
        INVOKE  GetSystemMetrics,SM_CYSIZEFRAME
        pop     edx
        shl     eax,1
        pop     ecx
        add     eax,edx
        add     eax,ecx
        mov     dwMinHeight,eax
        INVOKE  LoadMenu,wc.hInstance,IDM_CTXT_MENU
        mov     hmenuContext,eax
        INVOKE  GetSubMenu,eax,0
        mov     hmenuCtxtPopup,eax
        xor     eax,eax                                               ;return 0

    .elseif eax==WM_CLOSE
        INVOKE  DestroyWindow,hWnd
        xor     eax,eax                                               ;return 0

    .elseif eax==WM_DESTROY
        INVOKE  DestroyMenu,hmenuContext
        INVOKE  PostQuitMessage,NULL
        xor     eax,eax                                               ;return 0

    .else
        INVOKE  DefWindowProc,hWnd,uMsg,wParam,lParam

    .endif
    ret

WndProc ENDP

;***********************************************************************************************

storeWindow   proc

        finit
        mov store.goodsPtr,offset storeGoods
        mov store.goodsCount,lengthof storeGoods
        mov store.customersPtr,offset storeCustomers
        mov store.customersCount,lengthof storeCustomers
        INVOKE  InitCommonControlsEx,offset icc

;------------------------------

;register the window class

        xor     edi,edi                                 ;EDI = 0
        mov     esi,offset wc                           ;ESI = offset wc
        INVOKE  GetModuleHandle,edi
        mov     [esi].WNDCLASSEX.hInstance,eax
        xchg    eax,ebx                                 ;EBX = wc.hInstance
        INVOKE  LoadIcon,ebx,IDI_ICON
        mov     [esi].WNDCLASSEX.hIcon,eax
        mov     [esi].WNDCLASSEX.hIconSm,eax
        INVOKE  LoadCursor,edi,IDC_ARROW
        mov     [esi].WNDCLASSEX.hCursor,eax
        INVOKE  RegisterClassEx,esi

;------------------------------

;load the main menu and create the window

        INVOKE  LoadMenu,ebx,IDM_MAIN_MENU
        mov     hmenuMain,eax
        INVOKE  CreateWindowEx,edi,offset szClassName,offset szAppName,
                WS_OVERLAPPEDWINDOW or WS_VISIBLE or WS_CLIPCHILDREN,
                CW_USEDEFAULT,SW_SHOWNORMAL,MAIN_WIDTH,MAIN_HEIGHT,edi,eax,ebx,edi
        INVOKE  UpdateWindow,eax


        mov     esi,offset msg                          ;ESI = msg structure address
        jmp short mLoop1

mLoop0: INVOKE  TranslateMessage,esi
        INVOKE  DispatchMessage,esi

mLoop1: INVOKE  GetMessage,esi,edi,edi,edi
        inc     eax                                     ;exit only
        shr     eax,1                                   ;if 0 or -1
        jnz     mLoop0                                  ;otherwise, we loop


        INVOKE  ExitProcess,[esi].MSG.wParam

storeWindow endp

testRegs proc
    xor eax,eax
    mov ax,cs
    print str$(eax),13,10
    ret
testRegs endp

end storeWindow     ;去掉storeWindow几个字并在storeMenu.asm的这个位置加上'storeMenu'可展示shell控制台菜单


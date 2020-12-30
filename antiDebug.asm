.386
.model flat,stdcall
option casemap:none

include \masm32\include\windows.inc
include \masm32\include\user32.inc
include \masm32\include\kernel32.inc
includelib \masm32\lib\user32.lib
includelib \masm32\lib\kernel32.lib

returnMainMiddle proto

.const
    noDebugInfoFormat byte "看来你没有搞我啊",0
    isDebugInfoFormat byte "别搞我啊，你看看是不是有什么不对呢！",0

.code

isDebugInfo proc
    invoke MessageBox,NULL,addr isDebugInfoFormat,NULL,MB_OK
    ret
isDebugInfo endp

noDebugInfo proc
    invoke MessageBox,NULL,addr noDebugInfoFormat,NULL,MB_OK
    ret
noDebugInfo endp

antiDebug proc _lpExceptionRecord:ptr EXCEPTION_RECORD,_lpSEH:ptr SEH,_lpContext:ptr CONTEXT,_lpDispatcherContext:ptr DISPATCHER_CONTEXT
    mov esi,_lpExceptionRecord
    mov edi,_lpContext
    assume esi:ptr EXCEPTION_RECORD,edi:ptr CONTEXT
    invoke noDebugInfo
    invoke returnMainMiddle
    mov [edi].regEip,eax
    assume esi:nothing,edi:nothing
    mov eax,ExceptionContinueExecution
    ret
antiDebug endp


end
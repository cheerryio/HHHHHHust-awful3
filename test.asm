.386
.model flat,stdcall

.stack 4096

option nokeyword:<name>

.code

clear proc
	mov eax,0
	mov ebx,0
	ret
clear endp

end
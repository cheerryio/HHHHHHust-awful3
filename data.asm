
goodNum equ 10

store store_s < \
                "myStore",  \
                <"owner",,,1,>,    \
                ,   \
                ,   \
                ,  \
                >

storeCustomers user_s <"owner",,,1,>,<"user1","user1",,0,>,<"user2","user2",,0,>

storeGoods good_s <"pen",10,35 xor "C",80,70,25,3.2>,<"apple",9,12 xor "C",30,25,5,1.5>,     \
                  <"banana",10,35 xor "C",80,70,25,6.4>,<"ruler",9,12 xor "C",30,25,5,1.2>,       \
                  <"fish",10,35 xor "C",80,70,25,10.9>
rangeGoodBuffer byte goodNum dup(0)
inputBuffer byte 50 dup(0)
inputBufferLen dword ?
inputNameBuffer byte 50 dup(0)
inputNameBufferLen dword ?
inputPasswordBuffer byte 50 dup(0)
inputPasswordBufferLen dword ?
loginUser user_s <"nologin","nologin",,0,>
encryBossPassword db "o" xor "C","w" xor "C","n" xor "C","e" xor "C","r" xor "C",0

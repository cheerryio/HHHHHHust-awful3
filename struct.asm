
user_s struct
    name byte 20 dup(0)
    password byte 20 dup(0)
    islogin byte 0
    auth byte ?
    chosenGood dword 0
user_s ends

good_s struct
    name byte 20 dup(0)
    discount dword ?
    inPrice dword ?
    sellPrice dword ?
    inTotal dword ?
    sellTotal dword ?
    recommendIndex real8 ?
good_s ends


store_s struct
    name byte 20 dup(0)
    owner user_s <>
    customersPtr dword ?
    customersCount dword ?
    goodsPtr dword ?
    goodsCount dword ?
store_s ends
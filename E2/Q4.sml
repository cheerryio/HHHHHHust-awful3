(*
定义函数divisibleByThree: int -> bool
以使当n为3的倍数时
divisibleByThree n为true
否则为false。注意：程序中不能使用取余函数’mod’。
*)

(* divisibleByThree : int -> bool 	*)
(* REQUIRES: n>=0				*)
(* ENSURES: divisibleByThree n evaluates to true if n is a multiple of 3 and to false otherwise *)

fun divisibleByThree1(0:int):bool = true
|   divisibleByThree1(x:int):bool = if x < 0 then false else divisibleByThree(x-3);

divisibleByThree1(18);
divisibleByThree1(19);

(* 对负数的处理 *)
fun divisibleByThree2(~2:int):bool = false
|   divisibleByThree2(~1:int):bool = false
|   divisibleByThree2(0:int):bool = true
|   divisibleByThree2(x:int):bool=divisibleByThree2(x-3);

divisibleByThree2(18);
divisibleByThree2(19);

(*
函数evenP为偶数判断函数
即当且仅当该数为偶数时返回true
*)

(* evenP : int -> bool 		*)
(* REQUIRES: n >= 0 		*)
(* ENSURES: evenP n evaluates to true iff n is even. *)
fun evenP (0 : int) : bool = true
|   evenP 1 = false
|   evenP n = evenP (n - 2)


(*
试编写奇数判断函数oddP: int -> bool
当且仅当该数为奇数时返回true
注意：代码不要调用函数evenP或mod。
*)

fun oddP(~1:int):bool = true
|   oddP(0:int):bool = false
|   oddP(x:int):bool = oddP(x-2);

oddP(5);
oddP(6);
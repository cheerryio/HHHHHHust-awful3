(*  *)

(* mult’ : int list * int -> int *)
(* REQUIRES: true *)
(* ENSURES: mult’(L, a) 数组内所有元素乘积*a *)

fun mult'([], a) = a
|    mult'(x::L, a) = mult'(L,x * a);
mult'([1,2,3],4);

(*
利用mult’定义函数Mult’ : int list list * int -> int
使对任意整数列表的列表R和整数a
该函数用于计算a与列表R中所有整数的乘积
该函数框架如下所示，试完成代码的编写
*)
fun Mult'([], a) = a
|   Mult'(r::R,a) = let
                        fun subMult' [] = 1
                        |   subMult'(r::L) = r*subMult'(L);
                    in
                        subMult'(r) * Mult'(R,a)
                    end;
Mult'([[1,2,3],[4,5,6],[7,8,9]],5);


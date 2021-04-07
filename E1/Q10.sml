(* 编写递归函数square实现整数平方的计算，即square n = n * n
   要求：程序中可调用函数double，但不能使用整数乘法（*）运算 *)

(* double : int -> int *)
(* REQUIRES: n >= 0 *)
(* ENSURES: double n evaluates to 2 * n.*)

fun double(0 : int) : int = 0
|   double n = 2 + double (n - 1)

fun square 0 = 0
|  square n = square(n-1)+double(n)-1;

square(5);
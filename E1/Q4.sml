(* pi: real *)
val pi : real = 3.14159;

(* fact:int -> int *)
(* 没有考虑传参为负数的情况 *)
fun fact(0:int):int=1
|   fact(n)=n*fact(n-1);
fact(5);

(* f : int -> int *)
fun f (3 : int) : int = 9
|   f _ = 4;
f(3);
f(5);

(* circ : real -> real *)
fun circ (r : real) : real = (real 2) * pi * r;
circ(5.0);

(* semicirc : real -> real *)
fun semicirc(r:real) : real = pi * r;
semicirc(5.0);

(* area : real -> real *)
fun area (r : int) : real = pi * (real r) * (real r);
area(5);
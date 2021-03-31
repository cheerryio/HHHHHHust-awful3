(* sum : int list -> int 		*)
(* REQUIRES: true		*)
(* ENSURES: sum(L) evaluates to the sum of the integers in L. *)
fun sum [] = 0
|   sum (x ::L) = x + (sum L);
sum([1,2,3,4]);

(* mult : int list -> int 		*)
(* REQUIRES: true		*)
(* ENSURES: mult(L) evaluates to the product of the integers in L. *)
fun mult [] = 1
|   mult (x ::L) = x * mult(L);
mult([1,2,3,4]);
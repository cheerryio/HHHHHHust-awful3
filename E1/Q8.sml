(* mult : int list list -> int 	*)
(* REQUIRES: true		*)
(* ENSURES: mult(R) evaluates to the product of all the integers in the lists of R. *)
 
fun Mult [] = 1
|   Mult((r::L)::R) = r*Mult(L::R)
|   Mult([]::R) = Mult(R);

Mult([[1,2,3],[1,2,5,4],[10]]);

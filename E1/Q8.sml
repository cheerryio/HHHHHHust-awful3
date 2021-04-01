(* mult : int list list -> int 	*)
(* REQUIRES: true		*)
(* ENSURES: mult(R) evaluates to the product of all the integers in the lists of R. *)
 
(* 对Mult1,Mult2的思路：一轮将一个数乘进总数 *)
fun Mult1 [] = 1
|   Mult1((r::L)::R) = r*Mult1(L::R)
|   Mult1(r::R) = Mult1(R);
Mult1([[1,2,3],[1,2,5,4],[10]]);

fun Mult2 [] = 1
|   Mult2([]::R) = Mult2(R)
|   Mult2((r::L)::R) = r*Mult2(L::R);
Mult2([[1,2,3],[1,2,5,4],[10]]);

(* Mult3思路：一轮中对一个int list求其元素的乘积 *)
fun Mult3 [] = 1
|   Mult3(L::R) = let 
                    fun subMult3 [] =1
                    |   subMult3(r::L) = r*subMult3(L);
                  in
                    subMult3(L)*Mult3(R)
                  end;
Mult3([[1,2,3],[1,2,5,4],[10,0]]);
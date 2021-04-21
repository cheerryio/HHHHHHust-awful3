(*
exists: (‘a -> bool) -> ‘a list -> bool
forall: (‘a -> bool) -> ‘a list -> bool
对函数p: t -> bool, 整数集L: t list,
有：
    exists p L =>* true if there is an x in L such that p x=true;
    exists p L =>* false otherwise.
    forall p L =>* true if p x = true for every item x in L;
    forall p L =>* false otherwise.
*)

fun exists(p:'a->bool):'a list->bool = 
    fn L =>
        case L of
            [] => false
        |   x::R => if p(x) then true else exists(p)(R);

fun forall(p:'a->bool):'a list->bool = 
    fn L =>
        case L of
            [] => true
        |   x::R => if p(x) then forall(p)(R) else false;

fun p(4:int):bool = true
|   p(n:int):bool = false;

val L1:int list=[1,2,3,4,5,6,7,8,9,10];
val L2:int list=[1,2,3,5,6,7,8,9,10];
val L3:int list=[4,4,4,4,4,4,4,4,4,4,4];

exists(p)(L1);  (*true*)
exists(p)(L2);  (*false*)

forall(p)(L2);  (*false*)
forall(p)(L3);  (*true*)
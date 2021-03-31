(* 下列模式能否与类型为int list的L匹配成功 *)

val myList:int list=[1,2,3,4,5,6];

(* x::L ok *)
val x::L=myList;

(* _::_ ok *)
val _::_=myList;

(* x::(y::L) ok *)
val x::(y::L)=myList;

(* (x::y)::L fail *)

(* [x,y] only 2 elem list *)
val myList:int list=[1,2];
val [x,y]=myList;
(* 试写出与下列表述相对应的模式。如果没有模式与其对应，试说明原因 *)

(* list of length 3 *)
val myList=[1,2,3];
val [x,y,z]=myList;
val x::L=myList;
val x::(y::(z::L))=myList;

(* lists of length 2 or 3 *)
val myList=[1,2];
val x::y::L=myList;

(* Non-empty lists of pairs *)
val myList=[(1,1.0),(2,2.0),(3,3.0)];
val (x1,y1)::(x2,y2)::L=myList;

(* Pairs with both components being non-empty lists *)
val myList:int list*int list=([1,2,3],[4,5]);
val (x,y)=myList;
val (x1::x2::L1,y1::y2::L2)=myList;
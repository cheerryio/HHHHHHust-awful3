(*
treeFilter: (‘a -> bool) -> ‘a tree -> ‘a option tree
将树中满足条件P（ ‘a -> bool ）的节点封装成option类型保留，否则替换成NONE。
*)

datatype 'a tree = Empty | Node of 'a tree * 'a * 'a tree;
datatype 'a option = None | Some of 'a;

fun split([]) = raise Fail "cant split empty list"
|   split(L:'a list) = 
        let
          val mid = (length L) div 2
          val L1 = List.take(L,mid)
          val x::L2 = List.drop(L,mid)
        in
          (L1,x,L2)
        end

fun listToTree([]:'a list):'a tree = Empty
|   listToTree(L:'a list):'a tree = 
        let
          val (L1,x,L2) = split(L)
        in
          Node(listToTree(L1),x,listToTree(L2))
        end

fun treeFilter(f:'a->bool):'a tree->'a option tree = 
    fn t =>
        case t of
            Empty => Empty
        |   Node(t1,x,t2) =>    if f(x)
                                then Node(treeFilter(f)(t1),Some(x),treeFilter(f)(t2))
                                else Node(treeFilter(f)(t1),None,treeFilter(f)(t2))

fun filter(4) = false
|   filter(n) = true;

val L:int list=[1,2,3,4,5,6,7,8,9];
val t=listToTree(L);

treeFilter(filter)(t);
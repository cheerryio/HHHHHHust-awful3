(*
编写函数listToTree: int list -> tree，将一个表转换成一棵平衡树

提示：可调用split函数，split函数定义如下：
如果L非空，则存在L1, x, L2，满足：
	split L = (L1, x, L2) 	且 
	L = L1 @ x :: L2 		且 
	length(L1)和length(L2)差值小于1。

*)

datatype tree=Empty | Node of tree*int*tree;

fun split([]) = raise Fail "cant split empty list"
|   split(L:int list) = 
        let
          val mid = (length L) div 2
          val L1 = List.take(L,mid)
          val x::L2 = List.drop(L,mid)
        in
          (L1,x,L2)
        end

fun listToTree([]:int list):tree = Empty
|   listToTree(L:int list):tree = 
        let
          val (L1,x,L2) = split(L)
        in
          Node(listToTree(L1),x,listToTree(L2))
        end

fun midTrav(Empty) = []
|   midTrav(Node(t1,x,t2)) = midTrav(t1) @ (x::midTrav(t2))

val t:tree=listToTree([1,2,3,4,5,6,7,8,9]);

midTrav(t);
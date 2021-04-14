(*
编写函数revT: tree -> tree，对树进行反转，使trav(revT t) = reverse(trav t)。（trav为树的中序遍历函数）
假设输入参数为一棵平衡二叉树，验证程序的正确性，并分析该函数的执行性能（work和span）
*)

(*
当树的深度（L）为0时，结果为Empty，显然正确。
当树的深度（L）为1时，结果为（root），显然正确
假设当树的深度（L）为n（ n >= 1 ）时，命题成立，即trav(revT t) = reverse(trav T)

当树的深度（L1）为 n + 1 时，有
    trav(revT t) => trav(revT rchild t) @ value t :: trav (revT lchild t)
                 => reverse(trav rchild t) @ value t :: reverse(trav lchild t)
                 => reverse(trav lchild t @ value t :: trav rchild t)
                 => reverse(trav t)


W(d) = C + W(d-1) + W(d-1)
     = C + C + C + W(d-2) * 4
     ......
     = C * 2^d
     = O(2^d) = O(n)

S(d) = C + S(d-1)
     = C + C + S(d-2)
     ......
     = C * d
     = O(d)
*)

use "C:\\Users\\22216\\source\\repos\\HHHHHHust-awful3\\E3\\Q1.sml";

datatype tree = Empty | Node of tree*int*tree;

fun reverse([]:int list):int list = []
|   reverse(x::L):int list = reverse(L) @ [x]

fun revT(Empty) = Empty
|   revT(Node(t1,x,t2)) = Node(revT(t2),x,revT(t1))

val L:int list=[1,2,3,4,5,6,7,8,9,10,11,12];
val t:tree=listToTree(L);
midTrav(revT(t));
reverse(midTrav(t));
midTrav(revT(t)) = reverse(midTrav(t));
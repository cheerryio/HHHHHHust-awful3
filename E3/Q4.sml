(*
when given two trees, returns a value of type order
based on which tree has a larger value at the root node
*)

use "C:\\Users\\22216\\source\\repos\\HHHHHHust-awful3\\E3\\Q1.sml";

fun treeCompare(Empty,Empty):order = EQUAL
|   treeCompare(Empty,Node(t3,x2,t4)):order = LESS
|   treeCompare(Node(t1,x1,t2),Empty):order = EQUAL
|   treeCompare(Node(t1,x1,t2),Node(t3,x2,t4)):order = Int.compare(x1,x2)

(*
REQUIRES the subtrees of t are both minheaps
ENSURES swapDown(t) = if t is Empty or all of t’s immediate children are empty then
just return t, otherwise returns a minheap which contains exactly the elements in t
*)

fun swapDown(Empty):tree = Empty
|   swapDown(Node(Empty,x,Empty)) = Node(Empty,x,Empty)

|   swapDown(Node(Node(t1,x1,t2),x,Empty)) = 
        if Int.compare(x,x1) = GREATER
        then Node(swapDown(Node(t1,x,t2)),x1,Empty)
        else Node(Node(t1,x1,t2),x,Empty)

|   swapDown(Node(Empty,x,Node(t3,x2,t4))):tree = 
        if Int.compare(x,x2) = GREATER
        then Node(Empty,x2,swapDown(Node(t3,x,t4)))
        else Node(Empty,x,Node(t3,x2,t4))

|   swapDown(Node(Node(t1,x1,t2),x,Node(t3,x2,t4))):tree = 
        if x>x1 orelse x>x2
        then
        if x1<x2
        then Node(swapDown(Node(t1,x,t2)),x1,Node(t3,x2,t4))
        else Node(Node(t1,x1,t2),x2,swapDown(Node(t3,x,t4)))
        else Node(Node(t1,x1,t2),x,Node(t3,x2,t4))

val t:tree=listToTree([1,2,3,4,5,6,7,8]);
midTrav(t);
val swapT=swapDown(listToTree([5,8,1]));
midTrav(swapT);

(*
given an arbitrary tree t, evaluates to a minheap with exactly the elements of t
*)

fun heapify(Empty):tree = Empty
|   heapify(Node(t1,x,t2)):tree = swapDown(Node(heapify(t1),x,heapify(t2)))

val heapifyT=heapify(t);
midTrav(heapifyT);

(*
SwapDown: 

W_swap(d)   = C + W_swap(d-1)
            = C + C + W_swap(d-2)
            ......
            = C * d

            = O(d)
S_swap(d)   = C + S_swap(d-1)
            = C + C + S_swap(d-2)
            ......
            = C * d
            = O(d)

heapify:

W_heap(d)   = W_swap(d) + C + W_heap(d-1) + W_heap(d-1)
            = W_swap(d) + C + W_swap(d-1) * 2 + C * 2 + W_heap(d-2) * 4
            ......
            = O(d) * 2 ^ d + C * 2 ^ d
            = O(d*2^d)

S_heap(d)   = S_swap(d) + C + S_heap(d-1)
            = S_swap(d) + C + S_swap(d-1) + C + S_heap(d-2)
            ......
            = O(d) * d + C * d
            = O(d^2)
*)
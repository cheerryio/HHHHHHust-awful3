(*
当有序整数序列（L）的长度为0时，有ins(x,[]) = [x]，结果为有序正数
序列。
当有序数列（L）的长度 >= 0 时：
    假设当列表长度为n（n>=0）时，ins(x,L)为有序的序列。

当列表(L1)长度为n+1时，ins(x,L1)  =>  ins(x,y::L)
    当 compare(x,y) = GREATER，即 x > y 时，
    有 y::ins(x,L)
        因为 L1是升序的，即∀ i ∊ L1 有 y <= i
        ins(x,L)，其中L的长度为n，得到的是一个有序序列，记为 L2
        L2 = L ∪ { x }
        则 ∀ i ∊ L2 都有 y <= i 所以 y::L2 是一个有序序列、
    当 compare(x,y) != GREATER，即 x <= y 时，
    有 x::y::L
    因为 x <= y 并且 L1 是一个有序序列，
    所以 x::y::L是一个有序序列。


得到排序后的升序序列
当整数序列（L1）的长度为 0 时，有 isort([]) = [] ，是一个有序排列。
当整数序列（L1）的长度 >= 0 时：
    假设当整数序列长度为n（n>=0）时，isort(L1)为有序的序列。

当序列（L1）的长度为n+1时，isort(L1) => isort(x::L) 
                                  => ins(x,isort L)
    此时列表L的长度为n ，即 isort L 的结果为长度为n的有序序列
    （记为L2）。
    由上文可知，ins(x,L2) 得到的时一个有序序列，
    所以 isort(L1) 的结果是有序序列
    
*)

type order=LESS | EQUAL | GREATER

fun compare(x:int,y:int):order = 
    if x < y then LESS else
    if y < x then GREATER else EQUAL;

compare(1,2);

fun insert(x,[]) = [x]
|   insert(x,y::L) = case compare(x,y) of
                        GREATER => y::insert(x,L)
                    |   _       => x::y::L;

insert(4,[1,2,3,5,8,9]);

fun insertSort([]) = []
|   insertSort(x::L) = insert(x,insertSort(L));

insertSort([5,1,2,10,7,5,3,11,15,20,0]);
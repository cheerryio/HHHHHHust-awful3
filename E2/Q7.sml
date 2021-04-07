(*
编写函数reverse和reverse’，要求：
函数类型均为：int list->int list，功能均为实现输出表参数的逆序输出；
函数reverse不能借助任何帮助函数；函数reverse’可以借助帮助函数，时间复杂度为O(n)。
*)

fun reverse([]:int list):int list = []
|   reverse(x::L) = reverse(L)@[x];

reverse([1,2,3,4,5,6,7,8,9]);

(* 找到列表中最后一个元素 *)
fun reverse'([]:int list):int list = []
|   reverse'(L) = 
      let
        fun reverseHelp'([]:int list,targetL:int list):int list = targetL
        |   reverseHelp'(x::L,targetL) = reverseHelp'(L,x::targetL)
      in
        reverseHelp'(L,[])
      end

reverse'([1,2,3,4,5,6,7,8,9]);
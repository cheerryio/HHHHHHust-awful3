(*
其功能是提取第一个string list中的第i个元素和第二个int list中的第i个元素组成结果list中的第i个二元组
如果两个list的长度不同，
则结果的长度为两个参数list长度的最小值。
*)
fun zip ((x::L1,y::L2):string list*int list) : (string * int) list = 
    (x,y)::zip(L1,L2)
|   zip _ =[];

zip (["aaa","bbb","ccc"],[1,2,3]);

(*
其功能是执行zip函数的反向操作
将二元组list中的元素分解成两个list
第一个list中的元素为参数中二元组的第一个元素的list
第二个list中的元素为参数中二元组的第二个元素的list
*)
fun unzip ((x,y)::L):string list*int list=
    let
        val (xs,xy)=unzip(L)
    in
        (x::xs,y::xy)
    end
|   unzip _ = ([],[]);
unzip([("aaa",1),("bbb",2),("ccc",3)]);
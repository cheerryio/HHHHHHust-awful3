(*
编写函数mapList’，要求：
① 函数类型为: (‘a -> ‘b) -> (‘a list -> ‘b list)；   
② 功能为实现整数集的数学变换(如翻倍、求平方或求阶乘)。
③ 比较函数mapList’和mapList，分析、体会它们有什么不同。

不同:感觉差不多
*)

use "Q1.sml";

fun mapList'(f:'a->'b):'a list -> 'b list = 
    fn L =>
        case L of
            [] => []
        |   x::R => f(x)::mapList'(f)(R)

val L:int list=[1,2,3,4,5,6,7,8,9];

mapList'(double)(L);
mapList'(square)(L);
mapList'(factor)(L);
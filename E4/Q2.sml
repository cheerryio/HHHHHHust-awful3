(*
编写函数mapList，要求：
函数类型为: ((‘a -> ‘b) * ‘a list) -> ‘b list；
功能为实现整数集的数学变换(如翻倍、求平方或求阶乘)。
*)

use "Q1.sml";

fun mapList(f:'a->'b,[]:'a list):'b list = []
|   mapList(f:'a->'b,x::L):'b list = f(x)::mapList(f,L);

val L:int list=[1,2,3,4,5,6,7,8,9];

mapList(double,L);
mapList(square,L);
mapList(factor,L);
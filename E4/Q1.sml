(*
编写函数thenAddOne，要求：
函数类型为: ((int ->int) * int) -> int；
功能为将一个整数通过函数变换(如翻倍、求平方或求阶乘)后再加1。
*)

fun thenAddOne(f:int->int,n:int):int = f(n) + 1;

fun double(n:int):int = n * 2;

fun square(n:int):int = n * n;

fun factor(0:int):int = 1
|   factor(n:int):int = n * factor(n-1);

thenAddOne(double,5);
thenAddOne(square,5);
thenAddOne(factor,5);
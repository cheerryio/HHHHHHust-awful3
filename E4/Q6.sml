(*
十进制数可表示为5410，而二进制数可表示为102. 其中右下标的’10’,’2’称为基数。通常，给定一个基数b和n个数字dndn-1…d1组成的字符串，该数字字符串的值可用公式计算：∑i=1nbi-1di. 如：5410=5*101+4*100=54,102=1*21+0*20=2. 
同时，任意一个数可以表示成b以内数字的int list形式。如11002=[0,0,1,1], 5410=[4,5].

(1)编写高阶函数：toInt: int -> int list -> int.
    对所有b>1和所有L: int list,如果L是一个b进制数的int list表示，函数toInt b L为其相应的整数值，
    toInt b的结果类型为：int list -> int.
    如：val base2ToInt = toInt 2;
        val 2 = base2ToInt [0,1];
*)

fun toInt(b:int):int list->int = 
    if b < 2
    then raise Fail "base cant below 2"
    else
        let
            fun toIntHelper(biminus1:int,b:int,sum:int,[]):int = sum
            |   toIntHelper(biminus1:int,b:int,sum:int,x::L):int = toIntHelper(biminus1*b,b,sum+x*biminus1,L)
        in
            fn L => toIntHelper(1,b,0,L)
        end;

toInt(3);
toInt(2)([0,0,1,1]);
toInt(10)([4,5]);

(*
(2) 利用数学操作mod和div可以将任意十进制整数n表示成基于基数b的b进制数形式，如4210=1325。
 编写高阶函数  toBase: int -> int -> int list 实现该转换：toBase b n将十进制数n转换为b进制数的int list表述形式（b>1, n≥0）。
*)

fun toBase(b:int):int->int list = 
    if b < 2
    then raise Fail "base cant below 2"
    else
        fn n =>
            if n = 0
            then []
            else (n mod b)::toBase(b)(n div b);

toBase(10)(54);
toBase(2)(36);

(*
(3)编写高阶函数    convert: int * int -> int list -> int list
对任意b1, b2 > 1和所有L: int list（L为一个b1进制数的int list表述形式），函数convert(b1, b2) L将b1进制数的int list表述L转换成b2进制数的int list表述，即满足 toInt b2 (convert(b1, b2) L) = toInt b1 L。
*)

fun convert(b1:int,b2:int):int list->int list = 
    if b1 < 2 orelse b2 < 2
    then raise Fail "b1 and b2 cant below 2"
    else fn L => toBase(b2)(toInt(b1)(L));

convert(10,2)([6]);

(*
试问：

第4行中的x、第5行中的m和第6行中的x的声明绑定的类型和值分别为什么
答：第四行x类型int，值2；第五行类型real，值12.4；第六行类型int，值9001

第14行表达式assemble(x, 3.0)计算的结果是什么？
答：27
*)
val x:int=3;
val temp:int=x+1;
fun assemble(x:int,y:real):int=
    let val g:real =let val x:int=2
                        val m:real=6.2*(real x)
                        val x:int=9001;
                        val y:real=m*y;
                    in y-m
                    end
    in x+(trunc g)
    end
val z=assemble(x,3.0);
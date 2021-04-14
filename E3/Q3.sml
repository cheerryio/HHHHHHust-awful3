(*
编写函数binarySearch: tree * int -> bool
当输出参数1为有序树时，如果树中包含值为参数2的节点，则返回true；否则返回false。
要求：程序中请使用函数Int.compare（系统提供），不要使用<, =, >。
*)

use "C:\\Users\\22216\\source\\repos\\HHHHHHust-awful3\\E3\\Q1.sml";

fun binarySearch(Empty,n:int):bool = false
|   binarySearch(Node(t1,x,t2),n:int):bool = 
        case Int.compare(x,n) of
            GREATER => binarySearch(t1,n)
        |   EQUAL => true
        |   LESS => binarySearch(t2,n)

binarySearch(t,9);
binarySearch(t,10);
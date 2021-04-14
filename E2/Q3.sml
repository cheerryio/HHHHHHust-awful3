(*
fun fib n = if n<=2 then 1 else fib(n-1) + fib(n-2);
当 n >= 2 时 
W_fib(n)    = W_fib(n-1) + W_fib(n-2) + C
            = W_fib(n-2) + W_fib(n-3) + W_fib(n-3) + 
                W_fib(n-4) + 2 * C + C
            ......
            = 2^n * C_n 
            = O(2^n)

W_fib(0) = C_0
W_fib(n) = W_fib(n-1) + C
            = W_fib(n-2) + C + C
            ......
            = n*C + C_0
            = O(n)
*)

fun fib1(n) = if n <= 2 then 1 else fib1(n-1)+fib1(n-2);

fib1(10);

fun fib2(1:int):int*int = (1,1)
|   fib2(n:int):int*int = 
        let
          val (x:int,y:int) = fib2(n-1);
        in
          (y,x+y)
        end;

fib2(10);
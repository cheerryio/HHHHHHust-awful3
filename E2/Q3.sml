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
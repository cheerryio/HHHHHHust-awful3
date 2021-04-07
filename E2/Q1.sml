
(* int * string list -> string list *)
fun all(your,base) = 
    case your of
        0 => base
    |   _ => "are belong to us"::all(your-1,base);

all(4,["haha"]);

(* fn:('a * int -> int) * 'a list -> int *)
fun funny(f, []) = 0
|   funny(f, x::xs) = f(x, funny(f, xs));

funny(fn (x,y)=>x+y,[1,2,3,6]);

(* 'a -> fn:('b -> 'a) *)
(fn x => (fn y => x))("Hello,World!")("Whatever it is hello world");
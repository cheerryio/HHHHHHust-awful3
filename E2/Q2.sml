type order=LESS | EQUAL | GREATER

fun compare(x:int,y:int):order = 
    if x < y then LESS else
    if y < x then GREATER else EQUAL;

compare(1,2);

fun insert(x,[]) = [x]
|   insert(x,y::L) = case compare(x,y) of
                        GREATER => y::insert(x,L)
                    |   _       => x::y::L;

insert(4,[1,2,3,5,8,9]);

fun insertSort([]) = []
|   insertSort(x::L) = insert(x,insertSort(L));

insertSort([5,1,2,10,7,5,3,11,15,20,0]);
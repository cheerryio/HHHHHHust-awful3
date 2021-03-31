fun upto(m:int,n:int):int list=
    if m>n then []
    else m::upto(m+1,n);

fun maxl([m]) = m
  | maxl(m::n::ns)=if m>n then maxl(m::ns)
                   else maxl(n::ns);

maxl [5,7,1,2,4,0,5,8,98];
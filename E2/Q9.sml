(*
给定一个数组A[1..n]，前缀和数组PrefixSum[1..n]定义为：	PrefixSum[i] = A[0]+A[1]+...+A[i-1]；
例如：PrefixSum [ ] = [ ]
	  PrefixSum [5,4,2] = [5, 9, 11]
	  PrefixSum [5,6,7,8] = [5,11,18,26]

试编写：
函数PrefixSum: int list -> int list，
	要求：WPrefixSum(n) = O(n2)。(n为输入int list的长度)
(2) 函数fastPrefixSum: int list -> int list，
	要求： WfastPrefixSum(n) =O(n). 
		（提示：可借助帮助函数PrefixSumHelp）
*)

fun prefixSum([]:int list):int list = []
|   prefixSum(A:int list):int list = 
        let
            fun prefixSumHelp([]:int list,targetL:int list,sum:int) = targetL
            |   prefixSumHelp(x::L,targetL:int list,sum:int) = prefixSumHelp(L,targetL@[x+sum],x+sum)
        in
            prefixSumHelp(A,[],0)
        end;

prefixSum([5,6,7,8]);

fun fastPrefixSum([]:int list):int list = []
|   fastPrefixSum(A:int list):int list = 
        let
            fun fastPrefixSumHelp([]:int list,targetL:int list,sum:int) = targetL
            |   fastPrefixSumHelp(x::L,targetL:int list,sum:int) = fastPrefixSumHelp(L,(x+sum)::targetL,x+sum);
            fun reverse([]:int list):int list = []
            |   reverse(L) = 
                let
                    fun reverseHelp([]:int list,targetL:int list):int list = targetL
                    |   reverseHelp(x::L,targetL) = reverseHelp(L,x::targetL)
                in
                    reverseHelp(L,[])
                end;
        in
            reverse(fastPrefixSumHelp(A,[],0))
        end;

fastPrefixSum([5,6,7,8]);
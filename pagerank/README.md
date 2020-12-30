# page rank

![](https://wikimedia.org/api/rest_v1/media/math/render/svg/d1679eaf5d85a8a2970b2df8efcda6898e4ef31e)

## Transition Matrix
转移矩阵P，(pi,pj)是页面pj指向pi的总链接数，所以每一列之和是pj链出的总链接数。将每一列归一化，最终得到转移矩阵P。l(pi,pj)是 pj链向pi的总链接数/pj链出的总链接数

## d (damping factor)
阻尼系数d，代表用户浏览到一个网页后，继续浏览下一个网页的概率。default to 0.85。当d，用户继续浏览下一个网页，用转移矩阵P*R。当1-d，用户停止浏览下一个网页，将当前页面的1平均分给所有N个页面。为什么是1？因为转移矩阵一列之和为1。

## R
pagerank结果。不停迭代直到收敛
# SSSP (单源最短路径) with Dijkstra's algorithm

- 使用c语言多线程pthread.h
- 对于每一个线程，只需要在dist中begin和end区间进行一次计算，选出最短的select。在多个线程结果融合的时候，选取距离最短的那个select
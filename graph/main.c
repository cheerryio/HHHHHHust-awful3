#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>
#define Thread_NUM 2//线程数

struct Para_type {
    int n;
    int** adj;
    int* dist;
    int* visited;
    int begin, end;
    int select;
};

void thread(void* arg)
{
    struct Para_type* ar = (struct Para_type*)arg;
    int n = ar->n;
    int** adj = ar->adj;
    int* dist = ar->dist;
    int* visited = ar->visited;
    // begin end参数决定了遍历begin到end的点，而不是0到n的点
    int begin = ar->begin;
    int end = ar->end;
    int select = ar->select;

    /*********  Begin  **********/
    //更新select顶点邻居的距离
    int j;
    for (j = begin; j < end; j++) {//更新邻居距离
        if (adj[select][j] != -1 && dist[select] + adj[select][j] < dist[j])
            dist[j] = dist[select] + adj[select][j];
    }
    //在未访问顶点中选择一个距离最近的顶点
    int min_dist = INT_MAX;
    int i;
    for (i = begin; i < end; i++) {//选择最近的顶点
        if (!visited[i] && dist[i] < min_dist) {
            min_dist = dist[i];
            select = i;
        }
    }
    /*********  End  **********/

    ar->select = select;
}

void computeSSSP(int** adj, int n) {
    //n为顶点数,adj为一个n*n的二维数组
    //adj[i][j]为顶点i到顶点j的距离，若adj[i][j]为-1，表示无法从i直接到达j
    //此外，adj[i][j]与adj[j][i]未必相等
    int source = 0;//设置顶点0为源点
    int* dist = (int*)malloc(n * sizeof(int));//dist[i]表示顶点i与顶点0的距离
    int* visited = (int*)malloc(n * sizeof(int));//visited[i]表示顶点i是否已访问
    struct Para_type ars[Thread_NUM];
    pthread_t pids[Thread_NUM];
    int i, j;
    int min_dist;

    for (i = 0; i < Thread_NUM; i++) {
        ars[i].n = n;
        ars[i].adj = adj;
        ars[i].dist = dist;
        ars[i].visited = visited;
    }

    for (i = 0; i < n; i++) {
        if (i == source) {
            dist[i] = 0;
            visited[i] = 1;
        }
        else {
            dist[i] = INT_MAX;
            visited[i] = 0;
        }
    }
    int select = source;
    int count = 0;
    while (count < n) {
        /*********  Begin  **********/
        //利用多线程，实现每个线程选择了一个局部最近的顶点，选择需要在其中选择一个全局最近的顶点
        int ret;
        int slice;
        slice = n / Thread_NUM;
        for (i = 0; i < Thread_NUM; i++) {
            ars[i].select = select;
            ars[i].begin = i * slice;
            if (i == Thread_NUM - 1)
                ars[i].end = n;
            else
                ars[i].end = (i + 1) * slice;
            ret = pthread_create(&(pids[i]), NULL, &thread, &(ars[i]));
        }
        for (i = 0; i < Thread_NUM; i++)
            pthread_join(pids[i], NULL);

        //ars中的每个ar.select都是对应线程的选择
        min_dist = INT_MAX;
        for (i = 0; i < Thread_NUM; i++) {
            if (dist[ars[i].select] < min_dist) {
                min_dist = dist[ars[i].select];
                select = ars[i].select;
            }
        }

        /*********  End  **********/

        visited[select] = 1;//更新最近顶点的状态为已访问
        count++;//访问数加一
    }
    for (i = 0; i < n; i++) {
        printf("%d", dist[i]);
        if (i < n - 1) printf(" ");
        else printf("\n");
    }
}

int main()
{
    FILE* fp = fopen("file5", "r");
    int i, j;
    int n;
    int m;
    int** adj;
    fscanf(fp, "%d", &n);
    adj = (int**)malloc(n * sizeof(int*));
    for (i = 0; i < n; i++) {
        adj[i] = (int*)malloc(n * sizeof(int));
    }
    fscanf(fp, "%d", &m);
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            adj[i][j] = -1;
        }
    }
    for (i = 0; i < m; i++) {
        int src, dst, weight;
        fscanf(fp, "%d", &src);
        fscanf(fp, "%d", &dst);
        fscanf(fp, "%d", &weight);
        adj[src][dst] = weight;
    }
    computeSSSP(adj, n);
    return 0;
}

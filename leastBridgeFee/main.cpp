
#include <cstdio>
#include <cstdlib>

#include <iostream>

#pragma warning (disable: 4996)

#define MAXN 6000
using namespace std;

int bingchaji[MAXN], Rank[200];
typedef struct {
    int a, b;
    double distance;
}Node;
Node a[MAXN];

typedef struct {
    int x;
    int y;
}Location;
Location locations[110];

int cmp(const void* a, const void* b) {
    if (((Node*)a)->distance - ((Node*)b)->distance > 0)
        return 1;
    return -1;
}

void Init(int n) {
    for (int i = 0; i <= n; i++) {
        bingchaji[i] = i;
    }
}

int find(int x) {
    int root = x;
    while (root != bingchaji[root]) root = bingchaji[root];
    return root;
}

void unite(int x, int y) {
    x = find(x);
    y = find(y);
    if (x != y)
        bingchaji[x] = y;
}
//n为边的数量，m为村庄的数量
double Kruskal(int n, int m) {
    int nEdge = 0;
    double res = 0;
    //将边按照权值从小到大排序
    qsort(a, n, sizeof(Node), cmp);
    for (int i = 0; i < n && nEdge != m - 1; i++) {
        //判断当前这条边的两个端点是否属于同一棵树
        if (a[i].distance < 10 || a[i].distance>1000)
            continue;
        if (find(a[i].a) != find(a[i].b)) {
            unite(a[i].a, a[i].b);
            res += a[i].distance;
            nEdge++;
        }
    }
    if (nEdge != m - 1)
        return -1;
    return res;
}

int main() {
    int viledgeNumber, pathNumber,rounds;
    cin >> rounds;
    for (int round = 0; round < rounds; round++) {
        pathNumber = 0;
        cin >> viledgeNumber;
        for (int i = 0; i < viledgeNumber; i++) {
            scanf("%d%d", &locations[i].x, &locations[i].y);
        }
        for (int m = 0; m < viledgeNumber - 1; m++) {
            for (int n = m + 1; n < viledgeNumber; n++) {
                a[pathNumber].a = m;
                a[pathNumber].b = n;
                int xDistance = locations[m].x - locations[n].x;
                int yDistance = locations[m].y - locations[n].y;
                a[pathNumber].distance = sqrt(xDistance * xDistance + yDistance * yDistance);
                pathNumber++;
            }
        }

        Init(pathNumber);

        double ans = Kruskal(pathNumber, viledgeNumber);
        if (ans == -1) printf("oh!\n");
        else printf("%.1lf\n", ans*100);
    }
    return 0;
}

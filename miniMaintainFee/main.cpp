
#include <cstdio>
#include <cstdlib>

#include <iostream>

#pragma warning (disable: 4996)

#define MAXN 6000
using namespace std;

int bingchaji[MAXN], Rank[200];
typedef struct {
    int a, b, price;
}Node;
Node a[MAXN];

int cmp(const void* a, const void* b) {
    return ((Node*)a)->price - ((Node*)b)->price;
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
int Kruskal(int n, int m) {
    int nEdge = 0, res = 0;
    //将边按照权值从小到大排序
    qsort(a, n, sizeof(Node), cmp);
    for (int i = 0; i < n && nEdge != m - 1; i++) {
        //判断当前这条边的两个端点是否属于同一棵树
        if (find(a[i].a) != find(a[i].b)) {
            unite(a[i].a, a[i].b);
            res += a[i].price;
            nEdge++;
        }
    }
    return res;
}

int main() {
    int viledgeNumber, path,pathNumber, ans;
    char source, destination;
    while (scanf("%d", &viledgeNumber), viledgeNumber) {
        pathNumber = 0;
        for (int i = 0; i < viledgeNumber - 1; i++) {
            cin >> source;
            cin >> path;
            for (int i1 = 0; i1 < path; i1++) {
                a[pathNumber].a = source - 'A';
                cin >> destination;
                a[pathNumber].b = destination - 'A';
                cin >> a[pathNumber].price;
                pathNumber++;
            }
        }
        Init(viledgeNumber);
        ans = Kruskal(pathNumber, viledgeNumber);
        if (ans == -1) printf("?\n");
        else printf("%d\n", ans);
    }
    return 0;
}

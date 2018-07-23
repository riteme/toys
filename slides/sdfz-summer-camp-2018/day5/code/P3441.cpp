#include <cctype>
#include <cstdio>
#include <cstring>
#include <climits>

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#define BUFFERSIZE 65536
static size_t pos = BUFFERSIZE;
static char buf[BUFFERSIZE];

inline void _getchar(char &c) {
    if (pos == BUFFERSIZE) {
        pos = 0;
        fread(buf, 1, BUFFERSIZE, stdin);
    }
    c = buf[pos++];
}

inline void read(int &x) {
    x = 0;
    char c;
    do
        _getchar(c);
    while (!isdigit(c));
    do {
        x = x * 10 + (c - '0');
        _getchar(c);
    } while (isdigit(c));
}

#define NMAX 1000000

static int n, m, deg[NMAX + 10], id[NMAX + 10], cnt[NMAX + 10];
static vector<int> T[NMAX + 10];

void initialize() {
    read(n); read(m);
    for (int i = 1; i < n; i++) {
        int u, v;
        read(u); read(v);
        T[u].push_back(v);
        T[v].push_back(u);
        deg[u]++;
        deg[v]++;
    }
}

int main() {
    initialize();

    queue<int> q;
    for (int i = 1; i <= n; i++)
        if (deg[i] == 1) {
            id[i] = 1;
            q.push(i);
        }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        cnt[id[u]]++;
        for (int v : T[u]) {
            if (id[v]) continue;
            deg[v]--;
            if (deg[v] == 1) {
                id[v] = id[u] + 1;
                q.push(v);
            }
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; i++)
        ans += min(2 * m, cnt[i]);
    printf("%d\n", ans);

    return 0;
}

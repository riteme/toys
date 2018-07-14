#include <cstdio>
#include <cstring>

#include <queue>
#include <algorithm>

using namespace std;

#define NMAX 150000

static int n, cur;
static int seq[NMAX + 10];
static int A[NMAX + 10], B[NMAX + 10];
static priority_queue<int> q;

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d%d", A + i, B + i);
        seq[i] = i;
    }

    sort(seq + 1, seq + n + 1, [](int x, int y) {
        return B[x] < B[y];
    });

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        int x = seq[i];
        if (cur + A[x] <= B[x]) {
            ans++;
            cur += A[x];
            q.push(A[x]);
        } else if (A[x] < q.top()) {
            cur -= q.top() - A[x];
            q.pop();
            q.push(A[x]);
        }
    }
    printf("%d\n", ans);
    return 0;
}

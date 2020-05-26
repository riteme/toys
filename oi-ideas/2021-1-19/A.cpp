#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>

using namespace std;

using i64 = long long;

constexpr int NMAX = 5000;
constexpr i64 MOD = 998244353;

i64 qpow(i64 x, i64 k) {
	i64 r = 1;
	for ( ; k; k >>= 1) {
		if (k & 1)
			r = r * x % MOD;
		x = x * x % MOD;
	}
	return r;
}

i64 inv(i64 x) {
    return qpow(x, MOD - 2);
}

// x[1]..x[n + 1] must be distinct
void lagrange_interp(int n, i64 x[], i64 f[], i64 a[]) {
	static i64 p[NMAX + 10];
	memset(p, 0, sizeof(i64) * (n + 2));

	p[n + 1] = 1;
	for (int i = 1; i <= n + 1; i++)
	for (int j = n + 2 - i; j <= n + 1; j++) {
		p[j - 1] = (p[j - 1] - p[j] * x[i]) % MOD;
	}

	memset(a, 0, sizeof(i64) * (n + 1));
	for (int i = 1; i <= n + 1; i++) {
		i64 c = 1;
		for (int j = 1; j <= n + 1; j++) {
			if (i != j)
				c = c * (x[i] - x[j]) % MOD;
		}
		c = inv(c) * f[i] % MOD;

		i64 u = 0;
		for (int j = n + 1; j > 0; j--) {
			u = (p[j] + u * x[i]) % MOD;
			a[j - 1] = (a[j - 1] + c * u) % MOD;
		}
	}
}

void lagrange_interp_test() {
	i64 x[100], f[100], v[100];
	x[1] = 1; f[1] = 3* 1 - 2*1 + 7;
	x[2] = 2; f[2] = 3* 4 - 2*2 + 7;
	x[3] = 8; f[3] = 3*64 - 2*8 + 7;
	x[4] = 4; f[4] = 3*16 - 2*4 + 7;
	lagrange_interp(3, x, f, v);

	// out: 7 998244351 3 0
	for (int i = 0; i <= 3; i++) {
		if (v[i] < 0)
			v[i] += MOD;
		printf("%lld ", v[i]);
	}
	puts("");
}

struct Path {
	int u, v;
};

int n, q;
vector<int> G[NMAX + 10];
vector<Path> P[NMAX + 10];
int pa[NMAX + 10], dep[NMAX + 10];
int in[NMAX + 10], out[NMAX + 10];

void dfs(int x) {
	static int cnt;
	in[x] = ++cnt;

	for (int v : G[x]) if (v != pa[x]) {
		pa[v] = x;
		dep[v] = dep[x] + 1;
		dfs(v);
	}

	out[x] = cnt;
}

int lca(int u, int v) {
	if (dep[u] < dep[v])
		swap(u, v);

	while (dep[u] > dep[v]) {
		u = pa[u];
	}
	while (u != v) {
		u = pa[u];
		v = pa[v];
	}

	return u;
}

void initialize() {
	scanf("%d%d", &n, &q);
	for (int i = 1; i < n; i++) {
		int u, v;
		scanf("%d%d", &u, &v);
		G[u].push_back(v);
		G[v].push_back(u);
	}

	dfs(1);

	for (int i = 0; i < q; i++) {
		int u, v;
		scanf("%d%d", &u, &v);
		int p = lca(u, v);
		P[p].push_back({u, v});
	}
}

i64 f[NMAX + 10];
i64 w[NMAX + 10];

void modify(int r, i64 v) {
	for ( ; r; r -= r & -r) {
		w[r] = w[r] * v % MOD;
	}
}

void modify(int l, int r, i64 v) {
    modify(l - 1, inv(v));
    modify(r, v);
}

i64 query(int x) {
	i64 r = 1;
	for ( ; x <= n; x += x & -x) {
		r = r * w[x] % MOD;
	}
	return r;
}

void reset() {
	for (int i = 1; i <= n; i++) {
        f[i] = 1;
		w[i] = 1;
	}
}

void dp(int x, int k) {
    for (int v : G[x]) if (v != pa[x]) {
        dp(v, k);
        f[x] = f[x] * f[v] % MOD;
    }

    i64 g = f[x];
    for (auto &p : P[x]) {
        f[x] = (f[x] + k * g % MOD * query(in[p.u]) % MOD * query(in[p.v])) % MOD;
    }

    g = g * inv(f[x]) % MOD;
    modify(in[x], out[x], g);
}

i64 x[NMAX + 10], v[NMAX + 10], a[NMAX + 10];

int main() {
	initialize();

    int c = min(n, q);
	for (int k = 1; k <= c; k++) {
		reset();
		dp(1, k);
		x[k] = k;
		v[k] = f[1];
        //printf("f[%d] = %lld\n", k, (f[1] + MOD) % MOD);
	}

	x[c + 1] = 0;
	v[c + 1] = 1;
	lagrange_interp(c, x, v, a);

	for (int i = 1; i <= q; i++) {
		if (a[i] < 0)
			a[i] += MOD;
		printf("%lld ", a[i]);
	}
	puts("");

	return 0;
}


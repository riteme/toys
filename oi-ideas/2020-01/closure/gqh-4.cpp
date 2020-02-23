#include <bits/stdc++.h>
#define rep(i, l, r) for (register int i = l; i <= r; i++)
#define per(i, r, l) for (register int i = r; i >= l; i--)
#define srep(i, l, r) for (register int i = l; i < r; i++)
#define sper(i, r, l) for (register int i = r; i > l; i--)
#define maxn 5019
#define maxr 6000000
#define gc() ((p1 == p2 && (p2 = (p1 = buffer) + fread(buffer, 1, maxr, stdin), p1 == p2)) ? EOF : *p1++)
using namespace std;

int n, k;
char g[maxn][maxn];
int deg[maxn];
int lis[maxn];

struct ele{
	int id, deg;
	ele(int id, int deg) : id(id), deg(deg){}
	ele(){}
	bool operator < (const ele b) const{
		return (deg == b.deg) ? id < b.id : deg > b.deg;
	}
};
bool cmp(const int i, const int j){
	return deg[i] > deg[j];
}
set<ele> s;
set<ele> :: iterator it;

char buffer[maxr], *p1, *p2;
template<class T> void read(T& x){
	char ch = gc(); x = 0;
	while (!('0' <= ch && ch <= '9')) ch = gc();
	while ('0' <= ch && ch <= '9') x = (x << 1) + (x << 3) + ch - '0', ch = gc();
}

void reads(int i){
	char ch = gc(); int j = 0;
	while (!('0' <= ch && ch <= '1')) ch = gc();
	while ('0' <= ch && ch <= '1') {
		g[i][++j] = ch, ch = gc();
		if (i == j) continue;
		if (g[i][j] == '1') deg[i]++;
	}
}
int main(){
	//freopen("closure.in", "r", stdin);
	read(n), read(k);
	rep(i, 1, n) reads(i);
	//正向 310 反向 120
	int T = 20;
	while (T--){
		rep(i, 1, n) lis[i] = i;
		sort(lis + 1, lis + 1 + n, cmp);
		rep(j, 1, n){
			int i = lis[j];
			rep(y, 1, n){
				if (i == y || (g[i][y] = '1')) continue;
				if (deg[i] + deg[y] >= k){
					g[i][y] = g[y][i] = 1;
					deg[i]++, deg[y]++;
				}
				else break;
			}
		}
	}
	
	rep(i, 1, n) puts(g[i] + 1);
	return 0;
}

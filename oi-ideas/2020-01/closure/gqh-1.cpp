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
char ss[maxn];
int deg[maxn];
int node[maxn][maxn], over[maxn];

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

int spfa[maxn];
int head = 0, tail = 0;
bool vis[maxn];
bool cmp(const int i, const int j){
	return deg[i] > deg[j];
}
//int times = 0;
//int maxtime = 700000;
void SPFA(int x){
	//times++;
	//if (times >= maxtime) return;
	bool f = 0;
	int _n = n - over[x];
	//random_shuffle(node[x] + 1, node[x] + 1 + _n);
	rep(j, 1, _n) {
		int i = node[x][j];
		if (i == x) continue;
		if (g[x][i] == '1'){
			swap(node[x][j], node[x][_n]);
			over[x]++, _n--, j--;
			continue;
		}
		if (deg[i] + deg[x] >= k){
			f = 1;
			deg[i]++, deg[x]++;
			g[i][x] = g[x][i] = '1';
			swap(node[x][j], node[x][_n]);
			over[x]++, _n--, j--;
			if (!vis[i]) vis[i] = 1, SPFA(i), vis[i] = 0; 
			//SPFA(x);
		}
	}
	if (f) SPFA(x);
}
/*
void SPFA(){
	rep(i, 1, n) spfa[++head] = i, vis[i] = 1;
	while (tail != head){
		if (tail == maxn - 1) tail = 0;
		if (head == maxn - 1) head = 0;
		int x = spfa[++tail]; vis[x] = 0;
		rep(i, 1, n) {
			if (g[i][x] == '1' || i == x) continue;
			if (deg[i] + deg[x] >= k){
				deg[i]++, deg[x]++;
				g[i][x] = g[x][i] = '1';
				if (!vis[i]) spfa[++head] = i, vis[i] = 1; 
				if (!vis[x]) spfa[++head] = x, vis[x] = 1; 
			}
		}
	}
}
*/
int main(){
	//freopen("closure.in", "r", stdin);
	//freopen("closure.out", "w", stdout);
	read(n), read(k);
	rep(i, 1, n) rep(j, 1, n) node[i][j] = j;
	rep(i, 1, n) swap(node[i][i], node[i][n]), over[i] = 1;
	rep(i, 1, n) reads(i);
	rep(i, 1, n) {
		int _n = n - over[i];
		rep(j, 1, _n){
			if (g[i][node[i][j]] == '1') {
				swap(node[i][j], node[i][_n]);
				over[i]++, _n--, j--;
			}
		}
	}
	//rep(i, 1, n) lis[i] = i;
	//sort(lis + 1, lis + 1 + n, cmp);
	rep(i, 1, n) vis[i] = 1, SPFA(i), vis[i] = 0;
	rep(i, 1, n) puts(g[i] + 1);
	return 0;
}
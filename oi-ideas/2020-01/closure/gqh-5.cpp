#include <bits/stdc++.h>
#define rep(i, l, r) for (register int i = l; i <= r; i++)
#define per(i, r, l) for (register int i = r; i >= l; i--)
#define srep(i, l, r) for (register int i = l; i < r; i++)
#define sper(i, r, l) for (register int i = r; i > l; i--)
#define maxn 6019
using namespace std;

int n, k;
bool g[maxn][maxn];
char ss[maxn];
int deg[maxn];
struct ele{
	int id, deg;
	ele(int id, int deg) : id(id), deg(deg){}
	ele(){}
	bool operator < (const ele b) const{
		return (deg == b.deg) ? id < b.id : deg > b.deg;
	}
};
int lis[maxn];
bool cmp(const int i, const int j){
	return deg[i] > deg[j];
}
set<ele> s;
set<ele> :: iterator it;
int main(){
	scanf("%d%d", &n, &k);
	rep(i, 1, n){
		scanf("%s", ss + 1);
		rep(j, 1, n) {
			if (i == j) continue;
			g[i][j] = ss[j] - '0'; 
			if (g[i][j]) deg[i]++;
		}
	}
	rep(i, 1, n) lis[i] = i;
	sort(lis + 1, lis + 1 + n, cmp);
	rep(i, 1, n) s.insert(ele(i, deg[i]));
	rep(j, 1, n){
		int i = lis[j];
		s.erase(ele(i, deg[i]));
		it = s.begin();
		int x; ele op;
		while (it != s.end()){
			op = *it; it++;
			if (op.deg + deg[i] < k) break;
			x = op.id;
			if (g[x][i]) continue;
			g[x][i] = g[i][x] = 1;
			deg[x]++, deg[i]++;
			s.erase(op);
			op.deg++;
			s.insert(op);
		}
		s.insert(ele(i, deg[i]));
	}
	rep(i, 1, n) {rep(j, 1, n) printf("%c", (g[i][j]) ? '1' : '0'); printf("\n");}
	return 0;
}
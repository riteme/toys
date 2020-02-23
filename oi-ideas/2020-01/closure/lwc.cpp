#include <bits/stdc++.h>

using namespace std;

const int N = 5050;

int n, K, mx;		
int mat[N][N];
int d[N];
char st[N];
unordered_set<int> s[N << 1];

int cal(int x, int y){
	return n * (x - 1) + y;
}

int main(){
	scanf("%d%d",&n,&K);
	for (int i = 1; i <= n; ++i){
		scanf("%s",st + 1);
		for (int j = 1; j <= n; ++j) mat[i][j] = (st[j] == '1');
	}
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j <= n; ++j)
			if (mat[i][j]) ++d[i];
	int cnt = 0;
	for (int i = 0; i <= 2 * n - 2; ++i) s[i].clear();
	for (int i = 1; i <= n; ++i)
		for (int j = 1; j < i; ++j)
			if (!mat[i][j]){
//				printf("%d %d %d\n",i,j,cal(i,j));
				s[d[i] + d[j]].insert(cal(i,j));
				++cnt;
			}
	for (int i = 2 * n - 2; i; --i)
		if (!s[i].empty()){
			mx = i;
			break;
		}
	for (int i = 1; i <= cnt; ++i){
		if (mx < K) break;
		int now = *s[mx].begin();
		s[mx].erase(*s[mx].begin());
		int nx = now % n, ny = now / n + 1;
		if (nx == 0) nx = n;
		mat[nx][ny] = mat[ny][nx] = 1;
//		printf("  %d : %d %d %d\n",mx,ny,nx,now);
		for (int j = 1; j <= n; ++j){
			if (nx != j && !mat[nx][j]){
//				printf("%d -> %d   %d %d\n",d[nx] + d[j], d[nx] + d[j] + 1, nx, j); 
				s[d[nx] + d[j]].erase(cal(nx, j));
				s[d[nx] + d[j] + 1].insert(cal(nx, j));
				if (d[nx] + d[j] + 1 > mx) mx = d[nx] + d[j] + 1;
			}
			if (ny != j && !mat[ny][j]){
//				printf("%d -> %d   %d %d\n",d[ny] + d[j], d[ny] + d[j] + 1, ny, j); 
				s[d[ny] + d[j]].erase(cal(ny, j));
				s[d[ny] + d[j] + 1].insert(cal(ny, j));
				if (d[ny] + d[j] + 1 > mx) mx = d[ny] + d[j] + 1; 
			}
		}
		++d[nx];++d[ny];
		while (mx && s[mx].empty()) --mx;
		if (mx == 0) break;
	}
	for (int i = 1; i <= n; ++i){
		for (int j = 1; j <= n; ++j){
			printf("%d",mat[i][j]);
		}
		puts("");
	}
}
		
		

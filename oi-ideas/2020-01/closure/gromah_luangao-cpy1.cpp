#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
#define N 5000 + 5

int n, k, Deg[N], Ord[N];
bool Flag[N][N];

int main()
{
	scanf("%d %d", &n, &k);
	for (int i = 0; i < n; i ++)
	{
		static char s[N];
		scanf("%s", s);
		for (int j = 0; j < n; j ++)
		{
			Flag[i][j] = (s[j] - '0');
			Deg[i] += Flag[i][j];
		}
		Ord[i] = i;
	}
    int neo = 0;
	int stage = 0;
	bool updated = true;
	while (updated)
	{
		updated = false;
		stage++;
        // fputs("new stage\n", stderr);
		sort(Ord, Ord + n, [](int u, int v) {
			return Deg[u] > Deg[v];
		});
		for (int i = 0; i < n; i ++)
		{
			static int A[N], B[N];
			int sza = 0, szb = 0;
            // fprintf(stderr, "cur = %d\n", Ord[i] + 1);
			for (int j = i + 1; j < n; j ++)
			{
				if (!Flag[Ord[i]][Ord[j]] && Deg[Ord[i]] + Deg[Ord[j]] >= k)
				{
                    // fprintf(stderr, "%d - %d\n", Ord[i] + 1, Ord[j] + 1);
                    neo++;
					updated = true;
					Flag[Ord[i]][Ord[j]] = Flag[Ord[j]][Ord[i]] = 1;
					Deg[Ord[i]] ++, Deg[Ord[j]] ++;
					A[++ sza] = Ord[j];
				}
				else B[++ szb] = Ord[j];
			}
			for (int j = i + 1, ia = 1, ib = 1; j < n; j ++)
			{
				if (ia <= sza && (ib > szb || Deg[A[ia]] > Deg[B[ib]]))
					Ord[j] = A[ia ++];
				else Ord[j] = B[ib ++];
			}
		}
	}
    fprintf(stderr, "new: %d\n", neo);
	fprintf(stderr, "stage: %d\n", stage);
	for (int i = 0; i < n; i ++)
	{
		for (int j = 0; j < n; j ++)
			putchar('0' + Flag[i][j]);
		putchar('\n');
	}
	return 0;
}

#include "../../testlib.h"
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
#define N 5000 + 5

int n, k, Deg[N], Ord[N];
bool Flag[N][N];

int main()
{
	ascanf("%d %d", &n, &k);
	Eoln();
	CheckRange(n, 1, 5000);
	CheckRange(k, 0, 2 * n - 2);
	for (int i = 0; i < n; i ++)
	{
		static char s[N];
		ascanf("%s", s);
		if (i < n - 1)
			Eoln();
		else Eof();
		ASSERT(strlen(s) == n, "invalid length");
		for (int j = 0; j < n; j ++)
		{
			CheckRange(s[j], '0', '1');
			Flag[i][j] = (s[j] - '0');
			Deg[i] += Flag[i][j];
		}
		Ord[i] = i;
	}
	for (int i = 0; i < n; i ++)
		ASSERT(Flag[i][i] == 0, "invalid input matrix");
	for (int i = 0; i < n; i ++)
		for (int j = 0; j < n; j ++)
			ASSERT(Flag[i][j] == Flag[j][i], "invalid input matrix");
	for (int t = 0; t < 2; t ++)
	{
		sort(Ord, Ord + n, [](int u, int v) {
			return Deg[u] > Deg[v];
		});
		for (int i = 0; i < n; i ++)
		{
			static int A[N], B[N];
			int sza = 0, szb = 0;
			for (int j = i + 1; j < n; j ++)
			{
				if (!Flag[Ord[i]][Ord[j]] && Deg[Ord[i]] + Deg[Ord[j]] >= k)
				{
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
	for (int i = 0; i < n; i ++)
	{
		for (int j = 0; j < n; j ++)
			putchar('0' + Flag[i][j]);
		putchar('\n');
	}
	return 0;
}

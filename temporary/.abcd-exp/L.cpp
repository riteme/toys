#include <bits/stdc++.h>

using namespace std;

#define EPS 1e-13

int main() {
	int t;
	scanf("%d", &t);
	while (t--) {
		int a, b, c, d;
		scanf("%d%d%d%d", &a, &b, &c, &d);
		double x = a ? b * log(a) : -1e99, y = c ? d * log(c) : -1e99;
		if (fabs(x - y) < EPS) puts("Lazy");
		else if (x < y) puts("Congrats");
		else puts("HaHa");
	}
	return 0;
}

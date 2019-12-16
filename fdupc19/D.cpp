#include <bits/stdc++.h>
using namespace std;

int main() {
  int n, k, ans = 1;
  scanf ("%d%d", &n, &k);
  for (int i = 0, cur = 0, x; i < n; i++) {
    scanf("%d", &x);
    if (cur + x <= k) cur += x;
    else ans++, cur = x;
  }
  printf ("%d", ans);
  return 0;
}

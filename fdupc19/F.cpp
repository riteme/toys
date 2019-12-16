//#include<bits/stdc++.h>
#include<cstdlib>
#include<cstring>
#include<cstdio>
#include<cmath>
#define mod 998244353
#define N 500010
#define M 22
using namespace std;
inline int read()
{
	int x=0,f=1;char ch=getchar();
	while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
	while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
	return x*f;
}
int K,n;
int qm(int x,int y)
{
	int ret=1;
	while(y)
	{
		if(y&1)ret=1ll*ret*x%mod;
		x=1ll*x*x%mod;y>>=1;
	}
	return ret;
}
int f[M][N],cnt[M];
int pre[N],ans;
int C(int x,int y){return 1ll*pre[x]*qm(pre[y],mod-2)%mod*qm(pre[x-y],mod-2)%mod;}
int main()
{
	pre[0]=1;for(int i=1;i<N;i++)pre[i]=1ll*pre[i-1]*i%mod;
	n=read(),K=read();
	for(int j=1;j<=n;j++)f[1][j]=1;
	for(int i=1;i<M-1;i++)
		for(int j=1;j<=n;j++)
			for(int k=j+j;k<=n;k+=j)
				f[i+1][k]=(f[i+1][k]+f[i][j])%mod;
	for(int i=1;i<M;i++)
		for(int j=1;j<=n;j++)cnt[i]=(cnt[i]+f[i][j])%mod;
	for(int i=1;i<M&&i<=K;i++)ans=(ans+1ll*cnt[i]*C(K-1,i-1)%mod)%mod;
	printf("%d\n",ans);
}
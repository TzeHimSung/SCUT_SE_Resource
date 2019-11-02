#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

const int maxn=1e5+10;
const int mod=1e6; //模数很怪，如果对1e6+7取模，时间很差，如果对1e6取模就秒过，玄学OJ
int hashPos[maxn*10],sum[maxn][35],c[maxn][35];
int n,k,ans=0;

int hashKey(int *cc)
{
    int key=0;
    for (int j=1;j<k;j++) //第0位必然是0，没用，所以直接从第一位开始
        key=key%mod+cc[j]<<2; //先取模，再加上，再左移两位
    key=abs(key)%mod; //注意保证非负
    return key;
}

int main()
{
    memset(hashPos,-1,sizeof(hashPos));
    scanf("%d%d",&n,&k);
    hashPos[0]=0;
    for (int i=1;i<=n;i++)
    {
        int x;
        scanf("%d",&x);
        for (int j=0;j<k;j++)
        {
            sum[i][j]=sum[i-1][j]+x%2; //x%2是当前数的二进制表示的最后一位
            c[i][j]=sum[i][j]-sum[i][0];
            x>>=1; //别忘了右移
        }
        int key=hashKey(c[i]); //计算hash值
        while (hashPos[key]!=-1) //如果有重复
        {
            int j;
            for (j=0;j<k;j++)
                if (c[i][j]!=c[hashPos[key]][j]) //遍历一遍检查是否真的重复
                    break;
            if (j==k && ans<(i-hashPos[key])) //如果遍历完发现完全相同，而且答案更优，更新答案
            {
                ans=i-hashPos[key];
                break;
            }
            key++;
        }
        if (hashPos[key]==-1) hashPos[key]=i; //没重复就标记一下
    }
    printf("%d\n",ans);
    return 0;
}
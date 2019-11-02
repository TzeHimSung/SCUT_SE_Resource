#include <bits/stdc++.h>
#define pb push_back

using namespace std;

vector<int>ans;

void solve(int n)
{
    //对于任意一个丑数a，必定存在一个比它小的丑数b，满足a==b*2 || a==b*3 || a==b*5
    //又因为任意一个丑数a可以表示成a=2^x*3^y*5^z
    //故枚举因子幂次，从1开始往后构造
    int x,y,z; x=y=z=0;
    for (int i=1;i<n;i++)
    {
        int a=ans[x]*2, b=ans[y]*3, c=ans[z]*5;
        int num=min(min(a,b),c);
        if (num==a) x++;
        if (num==b) y++;
        if (num==c) z++;
        ans.pb(num);
    }
}

int main()
{
    ans.clear(); ans.pb(1);
    int n;
    printf("Please input the number N: ");
    scanf("%d",&n);
    solve(n);
    printf("The No.%d ugly number is: %d",n,ans[n-1]);
    return 0;
}
#include <bits/stdc++.h>

using namespace std;

const int maxn=1e4+10;
int n,x,y,cnt=1;
int m[maxn][maxn];

void findP(int x1, int x2, int y1, int y2, int &x, int &y)
{
    for (int i=x1;i<=x2;i++)
        for (int j=y1;j<=y2;j++)
            if (m[i][j])
            {
                x=i; y=j;
                return;
            }
}

void solve(int sideLen,int xStart, int yStart)
{
    //分治，把当前矩阵分为四个相同的子矩阵来处理
    //如果当前矩阵的边长为2，则直接填充
    //cnt记录是第几个填进去的L型块，初始被涂黑的格子被标记为1
    if (sideLen==2)
    {
        cnt++;
        for (int i=xStart;i<xStart+sideLen;i++)
            for (int j=yStart;j<yStart+sideLen;j++)
                if (!m[i][j]) m[i][j]=cnt;
        return;
    }
    int px,py;
    //找出当前矩阵中哪一个格子被填充，存在px py里
    findP(xStart,xStart+sideLen-1,yStart,yStart+sideLen-1,px,py);
    cnt++;
    //如果被填充的格子在左上角
    if (px<=xStart+sideLen/2-1 && py<=yStart+sideLen/2-1)
    {
        m[xStart-1+sideLen/2+1][yStart-1+sideLen/2]=cnt;
        m[xStart-1+sideLen/2][yStart-1+sideLen/2+1]=cnt;
        m[xStart-1+sideLen/2+1][yStart-1+sideLen/2+1]=cnt;
    }
    //如果被填充的格子在左下角
    else if (px>xStart+sideLen/2-1 && py<=yStart+sideLen/2-1)
    {
        m[xStart-1+sideLen/2][yStart-1+sideLen/2]=cnt;
        m[xStart-1+sideLen/2][yStart-1+sideLen/2+1]=cnt;
        m[xStart-1+sideLen/2+1][yStart-1+sideLen/2+1]=cnt;
    }
    //如果被填充的格子在右上角
    else if (px<=xStart+sideLen/2-1 && py>yStart+sideLen/2-1)
    {
        m[xStart-1+sideLen/2][yStart-1+sideLen/2]=cnt;
        m[xStart-1+sideLen/2+1][yStart-1+sideLen/2]=cnt;
        m[xStart-1+sideLen/2+1][yStart-1+sideLen/2+1]=cnt;
    }
    //如果被填充的格子在右下角
    else
    {
        m[xStart-1+sideLen/2][yStart-1+sideLen/2]=cnt;
        m[xStart-1+sideLen/2+1][yStart-1+sideLen/2]=cnt;
        m[xStart-1+sideLen/2][yStart-1+sideLen/2+1]=cnt;
    }
    //再分别处理四个子问题
    solve(sideLen/2,xStart,yStart);
    solve(sideLen/2,xStart+sideLen/2,yStart);
    solve(sideLen/2,xStart,yStart+sideLen/2);
    solve(sideLen/2,xStart+sideLen/2,yStart+sideLen/2);
}

void print()
{
    for (int i=1;i<=2*n;i++)
    {
        for (int j=1;j<=2*n;j++)
            printf("%4d",m[i][j]);
        puts("");
    }
}

int main()
{
    memset(m,0,sizeof(m));
    printf("Please input the size of the map: ");
    scanf("%d",&n);
    printf("Please input the location of the point: ");
    scanf("%d%d",&x,&y); m[x][y]=cnt;
    //这里输入n的时候出现了一点偏差，理解错题意了，这份代码里n是初始矩阵的半边长
    solve(2*n,1,1);
    print(); //打印答案
    return 0;
}
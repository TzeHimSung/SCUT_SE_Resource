#include <bits/stdc++.h>

using namespace std;

double find(const vector<int> &a,const vector<int> &b,int p,int q,int aRemain,int bRemain,int k)
{
    if (aRemain>bRemain)
        return find(b,a,q,p,bRemain,aRemain,k);
    if (!aRemain) //vector a is empty
        return b[q+k-1];
    if (k==1)
        return min(a[p],b[q]);
    int x=min(k/2,aRemain),y=k-x;
    if (a[p+x-1]<b[q+y-1])
        return find(a,b,p+x,q,aRemain-x,bRemain,k-x);
    else if (a[p+x-1]>b[q+y-1])
        return find(a,b,p,q+y,aRemain,bRemain-y,k-y);
    else return a[p+x-1];
}

double solve(const vector<int> &a,const vector<int> &b)
{
    int totalLen=a.size()+b.size();
    if (totalLen%2==0)
    {
        double x=find(a,b,0,0,a.size(),b.size(),totalLen/2);
        double y=find(a,b,0,0,a.size(),b.size(),totalLen/2+1);
        return (x+y)/2.0;
    }
    else
        return find(a,b,0,0,a.size(),b.size(),totalLen/2+1);
}

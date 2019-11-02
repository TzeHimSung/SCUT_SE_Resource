#include <bits/stdc++.h>
#define pb push_back

using namespace std;

vector<int>a,b;
const int maxn=1e2+10;

void insertSort(vector<int> &a)
{
    for (int i=1;i<a.size();i++)
    {
        for (int j=i; (j>0) && (a[j]>a[j-1]);j--)
            swap(a[j],a[j-1]);
    }
}

void bubbleSort(vector<int> &a)
{
    for (int i=0;i<a.size()-1;i++)
        for (int j=a.size()-1;j>i;j--)
            if (a[j]>a[j-1]) 
                swap(a[j],a[j-1]);
}

void selectSort(vector<int> &a)
{
    for (int i=0;i<a.size()-1;i++)
    {
        int index=i;
        for (int j=a.size()-1;j>i;j--)
            if (a[j]>a[index]) index=j;
        swap(a[i],a[index]);
    }
}

int calc(int *a, int l, int r, int &x)
{
    do{
        while (a[++l]>x);
        while ((l<r) && x>a[--r]);
        swap(a[l],a[r]);
    }while (l<r);
    return l;
}

void quickSort(int *a,int l, int r)
{
    if (r<=l) return;
    int mid=(l+r)/2;
    swap(a[mid],a[r]);
    int k=calc(a,l-1,r,a[r]);
    swap(a[k],a[r]);
    quickSort(a,l,k-1);
    quickSort(a,k+1,r);
}

void mergeSort(int *a, int *c, int l, int r)
{
    if (l==r) return;
    int mid=(l+r)/2;
    mergeSort(a,c,l,mid);
    mergeSort(a,c,mid+1,r);
    for (int i=l;i<=r;i++) //rebuild
        c[i]=a[i];
    int i1=l, i2=mid+1;
    for (int curr=l;curr<=r;curr++)
    {
        if (i1==mid+1) a[curr]=c[i2++];
        else if (i2>r) a[curr]=c[i1++];
        else if (c[i1]>c[i2]) a[curr]=c[i1++];
        else a[curr]=c[i2++];
    }
}

int main()
{
    printf("Please input the size of number: ");
    int n; scanf("%d",&n);
    printf("Please input elements: ");
    for (int i=1;i<=n;i++)
    {
        int x; scanf("%d",&x);
        a.pb(x);
    }
    b=a;

    // //insert sort
    insertSort(b);
    for (auto i:b) cout<<i<<" "; cout<<endl;
    b=a;

    // //bubble sort
    bubbleSort(b);
    for (auto i:b) cout<<i<<" "; cout<<endl;
    b=a;

    // //select sort
    selectSort(b);
    for (auto i:b) cout<<i<<" "; cout<<endl;
    b=a;

    //merge sort
    int c[maxn],d[maxn]; memset(c,0,sizeof(c)); memset(d,0,sizeof(d));
    int cnt=0;
    for (auto i:b) c[cnt++]=i;
    mergeSort(c,d,0,b.size()-1);
    for (int i=0;i<n;i++) cout<<c[i]<<" "; cout<<endl;

    //quick sort
    memset(c,0,sizeof(c)); memset(d,0,sizeof(d)); cnt=0;
    for (auto i:b) c[cnt++]=i;
    quickSort(c,0,b.size()-1);
    for (int i=0;i<n;i++) cout<<c[i]<<" "; cout<<endl;
    
    return 0;
}
#include <bits/stdc++.h>

using namespace std;

int main()
{
    //取前K个最小元素
    //用小根堆取K次就可以了，小根堆用STL的priority_queue实现
    priority_queue<int,vector<int>,greater<int>> ans;
    while (!ans.empty()) ans.pop();
    printf("Please input the size of number: ");
    int n; scanf("%d",&n);
    while (n--)
    {
        int x; scanf("%d",&x);
        ans.push(x);
    }
    printf("Please input k: ");
    int k; scanf("%d",&k);
    while (k--)
    {
        cout<<ans.top()<<" ";
        ans.pop();
    }
    return 0;
}
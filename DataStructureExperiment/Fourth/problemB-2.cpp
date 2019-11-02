#include <bits/stdc++.h>

using namespace std;

string s;
map<char,int>m;

int main()
{
    m.clear();
    cin>>s;
    //直接利用map进行哈希
    for (auto i:s)
        if (!m.count(i)) m[i]=1; else m[i]++;
    for (auto i:s)
        if (m[i]==1)
        {
            cout<<i<<endl;
            return 0;
        }
    puts("Do not fount the char only show once.");
    return 0;
}
#include <bits/stdc++.h>
#define mp make_pair
#define pb push_back

using namespace std;

struct Node
{
    int size, pos;

    Node(){}
    Node(int a,int b):size(a),pos(b){}
};

const int maxn = 1e5 + 10;
int n;
vector<Node>screw,nut; //screw存放螺钉信息，nut存放螺母信息
vector<pair<Node, Node> >ans; //ans存放答案


void print() //打印答案
{
    for (auto i : ans)
        printf("The No.%d screw fits to No.%d nut.\n", i.first.pos, i.second.pos);
}

void solve(const vector<Node> &sc, const vector<Node> &nu)
{
    //分治
    //对于当前要处理的螺钉和螺母，挑出一颗螺钉，把螺母分为三堆：大于、小于和等于
    //挑出一个一样大的螺母，把螺钉分为三堆
    //对于大小相等的螺钉和螺母，直接自由匹配，存入ans
    //然后再处理小于的两堆和大于的两堆
    vector<Node>sm1,sm2,sa1,sa2,bi1,bi2; //small same bigger 1是螺钉 2是螺母
    sm1.clear(); sm2.clear(); sa1.clear(); sa2.clear(); bi1.clear(); bi2.clear();
    Node std1=sc.front(); //随便拿一个螺钉
    for (auto i:nu) //遍历所有螺母，分类
    {
        if (i.size<std1.size) sm2.pb(i);
        else if (i.size==std1.size) sa2.pb(i);
        else bi2.pb(i);
    }
    Node std2=sa2.front(); //拿一个大小相同的螺母
    for (auto i:sc) //遍历所有螺钉，分类
    {
        if (i.size<std2.size) sm1.pb(i);
        else if (i.size==std2.size) sa1.pb(i);
        else bi1.pb(i);
    }
    for (int i=0;i<sa1.size();i++) //把大小相同的螺钉和螺母存入ans
        ans.pb(mp(sa1[i],sa2[i]));

    if (!sm1.empty()) solve(sm1,sm2); //分治
    if (!bi1.empty()) solve(bi1,bi2);
}

int main()
{
    ans.clear(); screw.clear(); nut.clear();
    printf("Please input the number: ");
    scanf("%d", &n);
    printf("Please input the size of screws: ");
    for (int i = 1; i <= n; i++)
    {
        int x; scanf("%d",&x);
        screw.pb(Node(x,i));
    }
    printf("Please input the size of nuts: ");
    for (int i = 1; i <= n; i++)
    {
        int x; scanf("%d",&x);
        nut.pb(Node(x,i));
    }
    solve(screw,nut); 
    print();
    return 0;
}
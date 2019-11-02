#include <bits/stdc++.h>
#define pb push_back
#define pf push_front

using namespace std;

struct Node
{
    char c;
    int time;
    Node() {}
    Node(char ch)
    {
        c = ch;
        time = 0;
    }
};

typedef struct listNode
{
    char c;
    int time;
} listnode;

const int maxn = 20;
const char visitSeq[] = {'D', 'H', 'H', 'G', 'H', 'E', 'G', 'H', 'G', 'H', 'E', 'C', 'E', 'H', 'G'};
// string visitSeq="FDFGEGFADFGE"; //这里用了书p321 例9.4的数据
Node listA[maxn];
int counter = 0;

int main()
{
    //referred to as count 根据访问次数排序
    listA[0].time = INT_MAX;
    for (int i = 1; i <= 8; i++)
        listA[i] = Node((char)('A' + i - 1));
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (visitSeq[i] == listA[j].c)
            {
                counter += j;
                listA[j].time++;
                int p = j;
                while (listA[p].time > listA[p - 1].time)
                {
                    swap(listA[p], listA[p - 1]);
                    p--;
                }
            }
        }
    }
    printf("the sequence after visit elements: ");
    for (int i = 1; i <= 8; i++)
        printf("%c ", listA[i].c);
    puts("");
    printf("the time is: %d\n", counter);

    //move to front 直接移到前端，使用了STL的list
    counter = 0; //总比较次数
    list<listnode> listB;
    for (int i = 1; i <= 8; i++)
    {
        listNode tmp;
        tmp.c = (char)('A' + i - 1);
        tmp.time = 0;
        listB.pb(tmp);
    }
    for (int i = 0; i < 12; i++)
    {
        int tmp = 0; //临时计数器
        for (list<listnode>::iterator it = listB.begin(); it != listB.end(); it++)
        {
            if (visitSeq[i] != it->c)
                tmp++;
            else
            {
                counter += tmp + 1;
                it->time++;
                listNode tmpN;
                tmpN.c = it->c;
                tmpN.time = it->time;
                listB.pf(tmpN);
                listB.erase(it);
                break;
            }
        }
    }
    printf("the sequence after visit elements: ");
    for (list<listnode>::iterator it = listB.begin(); it != listB.end(); it++)
        printf("%c ", it->c);
    puts("");
    printf("the time is: %d\n", counter);

    //transpose 相邻交换
    counter = 0;
    for (int i = 1; i <= 8; i++)
        listA[i] = Node((char)('A' + i - 1));
    for (int i = 0; i < 12; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (visitSeq[i] == listA[j].c)
            {
                counter += j;
                if (j != 1)
                    swap(listA[j], listA[j - 1]);
            }
        }
    }
    printf("the sequence after visit elements: ");
    for (int i = 1; i <= 8; i++)
        printf("%c ", listA[i].c);
    puts("");
    printf("the time is: %d\n", counter);
    return 0;

    //for referred to as count: HGFEDCBAABCDEFGH
    //for move to front: HGFEDCBAHGFEDCBA
    //for transpose: HGHGHGHGHGHGHGHG
}
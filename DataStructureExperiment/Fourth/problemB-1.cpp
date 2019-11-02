#include <bits/stdc++.h>

using namespace std;

const int maxn = 100;

struct HashNode
{
    int data;
    HashNode *nxt;
};

struct HashList //哈希表结构体
{
    HashNode *ele[maxn];
    int size;
};

int Hash(int size, int key) //哈希函数，直接对size取模就可以了
{
    return key % size;
}

HashNode *insertNode(HashNode *head, int key)
{
    if (head == nullptr) //如果没有元素，就新建节点
    {
        HashNode *p = new HashNode();
        p->data = key;
        p->nxt = nullptr;
        return p;
    }
    HashNode *it = head; //否则遍历
    while (it != nullptr) 
    {
        if (it->data == key) //找到就返回头指针
            return head;
        if (it->nxt == nullptr) //如果遍历完还没找到，新建节点
        {
            HashNode *p = new HashNode();
            p->data = key;
            p->nxt = nullptr;
            it->nxt = p;
            return head;
        }
        else
            it = it->nxt;
    }
}

int main()
{
    int a[10] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11};
    HashList hashList;
    hashList.size = 10;
    for (int i = 0; i < 10; i++)
        hashList.ele[i] = nullptr; //init
    for (int i = 0; i < 10; i++)
    {
        int hashVal = Hash(hashList.size, a[i]);
        printf("hashVal == %d, a[%d] == %d\n", hashVal, i, a[i]);
        hashList.ele[hashVal] = insertNode(hashList.ele[hashVal], a[i]);
    }
    puts("");
    for (int i = 0; i < 10; i++) //输出
    {
        printf("%d: ", i + 1);
        HashNode *p = hashList.ele[i];
        while (p != nullptr)
        {
            printf("%d ", p->data);
            p = p->nxt;
        }
        puts("");
    }
    return 0;
}
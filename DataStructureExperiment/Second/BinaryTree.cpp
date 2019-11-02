#include "BinaryTree.h"
#define pb push_back
#include <algorithm>
#include <string>

BinaryTree::BinaryTree(){}

//build function
BinaryTree::BinaryTree(const vector<string> &bt)
{
    buildTreeDFS(bt,0,head);
}

BinaryTree::BinaryTree(const vector<int> &num)
{
    head->hasValue=1;
    head->v=num[0];
    for (int i=1;i<num.size();i++)
        buildBST(num[i],head);
}

BinaryTree::~BinaryTree()
{
    deleteTreeDFS(head);
    delete head;
}

void BinaryTree::printPreOrder()const
{
    preOrderDFS(head);
}

void BinaryTree::printInOrder()const
{
    inOrderDFS(head);
}

void BinaryTree::printPostOrder()const
{
    postOrderDFS(head);
}

void BinaryTree::preOrderDFS(const Node *node)const
{
    if (node->hasValue) cout<<node->v<<" ";
    if (node->lson!=nullptr) preOrderDFS(node->lson);
    if (node->rson!=nullptr) preOrderDFS(node->rson);
}

void BinaryTree::inOrderDFS(const Node *node)const
{
    if (node->lson!=nullptr) inOrderDFS(node->lson);
    if (node->hasValue) cout<<node->v<<" ";
    if (node->rson!=nullptr) inOrderDFS(node->rson);
}

void BinaryTree::postOrderDFS(const Node *node)const
{
    if (node->lson!=nullptr) postOrderDFS(node->lson);
    if (node->rson!=nullptr) postOrderDFS(node->rson);
    if (node->hasValue) cout<<node->v<<" ";
}

//need to check
void BinaryTree::buildTreeDFS(const vector<string> &bt, int curPos, Node *node)
{
    bool negative=false;
    int tmp=0;
    if (bt[curPos][0]=='n') //null or nullptr
    {
        node->hasValue=false;
        return;
    }
    node->hasValue=true;
    for (auto i:bt[curPos])
    {
        if (i=='-') negative=true;
        else tmp=tmp*10+(int)i-48;
    }
    if (negative) tmp=0-tmp;
    node->v=tmp;
    if (curPos*2+1<bt.size())
    {
        Node *p=new Node();
        node->lson=p;
        buildTreeDFS(bt,curPos*2+1,p);
    }
    if (curPos*2+2<bt.size())
    {
        Node *p=new Node();
        node->rson=p;
        buildTreeDFS(bt,curPos*2+2,p);
    }
}

void BinaryTree::deleteTreeDFS(const Node *node)
{
    if (node->lson!=nullptr)
    {
        deleteTreeDFS(node->lson);
        delete node->lson;
    }
    if (node->rson!=nullptr)
    {
        deleteTreeDFS(node->rson);
        delete node->rson;
    }
}

//knowing preOrder and inOrder, get postOrder
Node* getPostOrder(const vector<int> &preOrder, const vector<int> &inOrder,int p, int q, int preStart)
{
    Node *re=new Node;
    re->hasValue=1;
    re->v=preOrder[preStart];
    if (p==q) goto mark;
    int index;
    for (index=preStart;;index++) if (inOrder[index]==preOrder[preStart]) break;
    if (index==p)
        re->lson=nullptr;
    else
        re->lson=getPostOrder(preOrder,inOrder,p,index-1,preStart+1);
    if (index==q)
        re->rson=nullptr;
    else
        re->rson=getPostOrder(preOrder,inOrder,index+1,q,preStart+index-p+1);
    mark:;
    cout<<re->v<<" ";
    return re;
}

void BinaryTree::buildBST(int value, Node *node)
{
    if (!node->hasValue)
    {
        node->hasValue=1;
        node->v=value;
        node->lson=nullptr;
        node->rson=nullptr;
        return;
    }
    if (value<node->v)
    {
        if (node->lson!=nullptr)
            buildBST(value,node->lson);
        else
        {
            Node *p=new Node();
            node->lson=p;
            buildBST(value,p);
        }
    }
    else
    {
        if (node->rson!=nullptr)
            buildBST(value,node->rson);
        else
        {
            Node *p=new Node();
            node->rson=p;
            buildBST(value,p);
        }
    }
}

Node* BinaryTree::findElement(const int &ele, Node *node)const
{
    if (node->v==ele)
        return node;
    if (ele<node->v)
    {
        if (node->lson==nullptr)
            return nullptr;
        else return findElement(ele,node->lson);
    }
    else
    {
        if (node->rson==nullptr)
            return nullptr;
        else return findElement(ele,node->rson);
    }
}

bool cmp(Node *a, Node *b)
{
    return (a->v)<(b->v);
}

//build huffmann tree
void BinaryTree::buildHuffmanTree(const vector<int> &a)
{
    vector<int>num; num.clear();
    for (auto i:a) num.pb(i);
    using std::sort;
    sort(num.begin(),num.end());
    Node *tmp1=new Node(); Node *tmp2=new Node();
    tmp1->v=num[0]; tmp1->hasValue=1;
    tmp2->v=num[1]; tmp2->hasValue=1;
    Node *p=new Node();
    p->lson=tmp1; p->rson=tmp2; p->v=tmp1->v+tmp2->v;
    vector<Node*>tmp;
    tmp.clear(); tmp.pb(p);
    for (int i=2;i<a.size();i++)
    {
        Node *p=new Node();
        p->hasValue=1; p->v=num[i];
        tmp.pb(p);
    }
    while (tmp.size()!=1)
    {
        sort(tmp.begin(),tmp.end());
        Node *p=new Node();
        p->lson=tmp[0]; p->rson=tmp[1];
        tmp.pb(p);
        tmp.erase(tmp.begin()); tmp.erase(tmp.begin());
    }
    head=tmp[0];
}

//print huffmann tree
void BinaryTree::printHuffmanTree(Node *node, string path)const
{
    if (node->hasValue)
    {
        cout<<node->v<<" "<<path<<endl;
        return;
    }
    if (node->lson!=nullptr)
        printHuffmanTree(node->lson,path+"0");
    if (node->rson!=nullptr)
        printHuffmanTree(node->rson,path+"1");
}

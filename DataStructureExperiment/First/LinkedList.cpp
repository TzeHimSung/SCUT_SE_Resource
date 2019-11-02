#include "LinkedList.h"

LinkList::LinkList(){}

LinkList::LinkList(int x)
{
    int num=x;
    head->nxt=tail;
    head->pre=nullptr;
    tail->pre=head;
    tail->nxt=nullptr; //init

    while (num)
    {
        Node *tmp=new Node();
        tmp->value=num%10;
        num/=10;
        tail->pre->nxt=tmp;
        tmp->pre=tail->pre;
        tail->pre=tmp;
        tmp->nxt=tail;
    }
    tail->nxt=nullptr;
    head->pre=nullptr;
}

LinkList::~LinkList()
{
    Node *p=head->nxt;
    while (p->nxt!=nullptr)
    {
        p=p->nxt;
        delete p->pre;
    }
    delete p;
    delete head;
}

void LinkList::print()const
{
    Node *p=head->nxt;
    while (p->nxt!=nullptr)
    {
        cout<<p->value;
        p=p->nxt;
    }
    cout<<endl;
}

void LinkList::removeElementFromBottom(int pos)
{
    Node *p1=head->nxt;
    int i;
    for (i=1;i<=pos-1 && p1->nxt!=nullptr;i++)
        p1=p1->nxt;
    if (i!=pos)
    {
        cout<<"Can not remove element at this position.\n";
        return;
    }
    Node *p2=head->nxt;
    while (p1->nxt->nxt!=nullptr)
    {
        p1=p1->nxt;
        p2=p2->nxt;
    }
    p2->pre->nxt=p2->nxt;
    p2->nxt->pre=p2->pre;

    delete p2;
}

LinkList* mergeLinkedList(const LinkList &lhs, const LinkList &rhs)
{
    Node *p=new Node();
    int tmp1=0,tmp2=0;
    p=lhs.head->nxt;
    while (p->nxt!=nullptr) p=p->nxt;
    p=p->pre;
    while (p->pre!=nullptr)
    {
        tmp1=tmp1*10+p->value;
        p=p->pre;
    }
    p=rhs.head->nxt;
    while (p->nxt!=nullptr) p=p->nxt;
    p=p->pre;
    while (p->pre!=nullptr)
    {
        tmp2=tmp2*10+p->value;
        p=p->pre;
    }
    LinkList *re=new LinkList(tmp1+tmp2);
    return re;
}

Node* LinkList::judgeCycle()const
{
    bool found=false;
    Node *p1=head->nxt;
    Node *p2=head->nxt;
    while (1)
    {
        p1=p1->nxt;
        p2=p2->nxt;
        if (p2->nxt!=nullptr) p2=p2->nxt;
        if (p2->nxt==nullptr) return nullptr;
        if (p1==p2) break;
    } //judge

    Node *pos=p1;
    p1=p1->nxt;
    int step=1;
    while (p1!=pos) p1=p1->nxt,step++;
    p2=head;
    while (p1!=p2)
    {
        for (int counter=1;counter<=step;counter++)
        {
            p1=p1->nxt;
            if (p1==p2) return p1;
        }
        p2=p2->nxt;
    }
}


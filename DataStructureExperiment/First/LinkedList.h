#ifndef LINKEDLIST_H_INCLUDED
#define LINKEDLIST_H_INCLUDED

#include <bits/stdc++.h>
#include "Node.h"

using namespace std;

class LinkList
{
public:
    int length;

    Node *head=new Node();
    Node *tail=new Node();

    LinkList();
    LinkList(int x);
    ~LinkList();

    void print()const;
    void removeElementFromBottom(int pos);
    Node* judgeCycle()const;
};

#endif // LINKEDLIST_H_INCLUDED

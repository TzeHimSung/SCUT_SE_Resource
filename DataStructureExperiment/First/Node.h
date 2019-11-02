#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

class Node
{
public:
    Node *pre=nullptr;
    Node *nxt=nullptr;
    int value;

    Node(){}
    Node(int v)
    {
        value=v;
    }
    ~Node(){}

    bool operator==(const Node *rhs)const
    {
        if (pre==rhs->pre && nxt==rhs->nxt)
            return true;
        else return false;
    }

    bool operator!=(const Node *rhs)const
    {
        if (pre!=rhs->pre || nxt!=rhs->nxt)
            return true;
        else return false;
    }
};

#endif // NODE_H_INCLUDED

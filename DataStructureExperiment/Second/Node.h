#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

class Node
{
public:
    Node *father;
    Node *lson,*rson;
    int v;
    bool hasValue;

    Node()
    {
        father=nullptr;
        lson=nullptr;
        rson=nullptr;
        v=0;
        hasValue=false;
    }

    Node(int x, Node *fa)
    {
        v=x;
        father=fa;
        lson=nullptr;
        rson=nullptr;
        hasValue=true;
    }

    ~Node(){}

    bool operator<(const Node &rhs)const
    {
        return v<rhs.v;
    }

};

#endif // NODE_H_INCLUDED

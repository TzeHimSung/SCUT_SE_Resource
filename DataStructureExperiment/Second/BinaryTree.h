#ifndef BINARYTREE_H_INCLUDED
#define BINARYTREE_H_INCLUDED

#include <iostream>
#include <vector>
#include "Node.h"

using namespace std;

class BinaryTree
{
public:
    Node *head=new Node();

    BinaryTree();
    BinaryTree(const vector<string> &bt);
    BinaryTree(const vector<int> &num);
    ~BinaryTree();

    void printPreOrder()const;
    void printInOrder()const;
    void printPostOrder()const;

    Node* findElement(const int &ele, Node *node)const;

    void buildHuffmanTree(const vector<int> &num);
    void printHuffmanTree(Node *node,string path)const;

private:
    void preOrderDFS(const Node *node)const;
    void inOrderDFS(const Node *node)const;
    void postOrderDFS(const Node *node)const;

    //need to check
    void buildTreeDFS(const vector<string> &bt, int curPos, Node *node);
    void deleteTreeDFS(const Node *node);

    void buildBST(int value, Node *node);
};

#endif // BINARYTREE_H_INCLUDED

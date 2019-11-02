#ifndef SEQUENCETABLE_H_INCLUDED
#define SEQUENCETABLE_H_INCLUDED

#include <bits/stdc++.h>

using namespace std;

class SequenceTable
{
public:
    static const int maxSize=1e5+10;
    int length;
    int data[SequenceTable::maxSize];

    SequenceTable();

    SequenceTable(int sizeOfTable);

    ~SequenceTable();

    void printAllElement();

    int find(int ele,int &save)const;

    int insertElement(int ele, int pos);

    int deleteElement(int pos);

    void clear();

    int isEmpty()const;
};

#endif // SEQUENCETABLE_H_INCLUDED

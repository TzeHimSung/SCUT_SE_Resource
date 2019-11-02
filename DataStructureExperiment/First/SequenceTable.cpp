#include "SequenceTable.h"

SequenceTable::SequenceTable()
{
    length=0;
    memset(data,0,sizeof(data));
}

SequenceTable::SequenceTable(int sizeOfTable)
{
    if (sizeOfTable<=0)
        cout<<"Can not build such a sequence table with this length.\n";
    length=sizeOfTable;
    for (int i=1;i<=length;i++) data[i]=0;
}

SequenceTable::~SequenceTable(){}

void SequenceTable::printAllElement()
{
    puts("The element of the sequence table is: ");
    for (int i=1;i<=length;i++) cout<<data[i]<<" ";
    puts("");
}

int SequenceTable::find(int ele, int &save)const
{
    for (int i=1;i<=length;i++)
    {
        if (data[i]==ele)
        {
            save=i;
            cout<<"The element is founded at position "<<save<<" .\n";
            return 1;
        }
    }
    cout<<"Can not fount element "<<ele<<" in this sequence table.\n";
    return 0;
}

int SequenceTable::insertElement(int ele, int pos)
{
    if (pos<=0 || pos>length+1)
    {
        cout<<"Can not insert element at this position.\n";
        return 0;
    }
    length++;
    for (int i=length;i>=1;i--)
    {
        data[i]=data[i-1];
        if (i-1==pos)
        {
            data[pos]=ele;
            cout<<"The element is inserted to sequence table successfully.\n";
            return 1;
        }
    }
    return 1;
}

int SequenceTable::deleteElement(int pos)
{
    if (pos<=0 || pos>length)
    {
        cout<<"Can not delete element at this position.\n";
        return 0;
    }
    for (int i=pos;i<=length;i++)
        data[i]=data[i+1];
    length--;
    cout<<"The element is deleted successfully.\n";
    return 1;
}

void SequenceTable::clear()
{
    for (int i=1;i<=length;i++) data[i]=0;
    length=0;
}

int SequenceTable::isEmpty()const
{
    if (!length) return 1;
        else return 0;
}

int mergeSequenceTable(const SequenceTable &lhs, const SequenceTable &rhs,SequenceTable &re)
{
    if (lhs.length+rhs.length>SequenceTable::maxSize)
    {
        cout<<"Can not merge these sequence tables, they are too large.\n";
        return 0;
    }
    int p=1,q=1;
    re.clear();
    re.length=lhs.length+rhs.length;
    for (int i=1;i<=re.length;i++)
        if (lhs.data[p]<=rhs.data[q])
            {
                re.data[i]=lhs.data[p];
                if (p<lhs.length) p++;
            }
            else
            {
                re.data[i]=rhs.data[q];
                if (q<rhs.length) q++;
            }
    cout<<"These sequence tables are merged to a new sequence table.\n";
    return 1;
}

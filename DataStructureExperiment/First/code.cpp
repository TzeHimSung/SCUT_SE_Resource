#include "SequenceTable.cpp"

using namespace std;

int main()
{
    SequenceTable a=SequenceTable(10);
    for (int i=1;i<=10;i++)
        a.data[i]=i;
    a.printAllElement();

    a.insertElement(10,2);
    a.printAllElement();

    SequenceTable b=SequenceTable(11);
    for (int i=11;i>0;i--)
        b.data[11-i+1]=i;

    SequenceTable c=SequenceTable();
    mergeSequenceTable(a,b,c); //c=a+b && c is sorted
    c.printAllElement();
    return 0;
}

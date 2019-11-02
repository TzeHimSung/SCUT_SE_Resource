#include "SequenceTable.cpp"
#include "LinkedList.cpp"
#include "addProblem.cpp"

using namespace std;

int main()
{
    SequenceTable a=SequenceTable(10);
    for (int i=1;i<=10;i++) a.data[i]=i;
    SequenceTable b=SequenceTable(10);
    for (int i=1;i<=10;i++) b.data[i]=10+i;
    b.printAllElement();
    SequenceTable c=SequenceTable();
    mergeSequenceTable(a,b,c);
    c.printAllElement();
    return 0;
}

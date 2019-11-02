#include <iostream>
#include <cstdio>
#include <ctime>
#define max_page_num 10

using namespace std;

int page[320]{0};
int pageFlu[320]{0};
int count=0,countPage=max_page_num,circle=1;
double lackPage=0;

struct Memo
{
    int num,a;
    Memo *nxt;
};

int judgePage(int value)
{
    return value/10;
}

int scanQueen(Memo *hear, int value)
{
    Memo *move=hear->nxt;
    while (move!=nullptr)
    {
        if (move->a==value)
            return 1;
        move=move->nxt;
    }
    return 0;
}

void print(Memo *hear)
{
    Memo *move=hear->nxt;
    while (move!=nullptr)
    {
        printf("%d ",move->a);
        move=move->nxt;
    }
    puts("");
}

void insert(Memo *hear, int value, int zl)
{
    if (countPage>=1)
    {
        Memo *move=hear->nxt;
        while (move->a!=-1)
            move=move->nxt;
        move->a=value;
        countPage--;
        printf("page %d in----order is: %d \n",value,zl);
    }
    else
    {
        Memo *move;
        move=hear->nxt;
        while (move->num!=circle)
            move=move->nxt;
        printf("page %d out, page %d in----order is: %d \n",move->a,value,zl);
        move->a=value;
        circle++;
        if (circle==max_page_num+1)
            circle=1;
    }
    print(hear);
}

void FIFO(Memo *hear)
{
    for (int i=0;i<320;i++)
    {
        if (scanQueen(hear,pageFlu[i])==0)
        {
            lackPage++;
            insert(hear,pageFlu[i],page[i]);
        }
        else
            printf("order %d to page %d is in memory\n",page[i],pageFlu[i]);
    }
}

void proPage()
{
    int m=rand()%320;
    page[count++]=m;
    if (count==320) return;
    int m_=0;
    m_=rand()%(m+1);
    page[count++]=m_;
    if (count==320) return;
    page[count++]=m_+1;
    if (count==320) return;
    int m__=0;
    m__=(m_+2)+rand()%(319-(m_+2)+1);
    page[count++]=m__;
    if (count==320) return;
    proPage();
}

void flu()
{
    for (int i=0;i<320;i++)
        pageFlu[i]=judgePage(page[i]);
}

int main()
{
    Memo stu[max_page_num+1];
    Memo *hear=&stu[0];
    for (int i=0;i<=max_page_num;i++)
    {
        if (i==max_page_num)
        {
            stu[i].a=-1;
            stu[i].nxt=nullptr;
            stu[i].num=i;
            break;
        }
        stu[i].nxt=&stu[i+1];
        stu[i].a=-1;
        stu[i].num=i;
    }
    srand(time(0));
    proPage();
    flu();
    FIFO(hear);
    printf("lacking times are: %0.0lf\n",lackPage);
    printf("aim percentage is %lf\n",1-lackPage/320);
    return 0;
}
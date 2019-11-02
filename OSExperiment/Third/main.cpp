#include <iostream>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <cstdio>
#define pb push_back 

using namespace std;

struct TimeSlice
{
    static const int maxLengthOfName=20;
    char name[maxLengthOfName];
    int arriveTime,remainTime,serveTime;
    char *state;
    TimeSlice *next;
};

TimeSlice* init()
{
    int numOfProcess,i;
    TimeSlice *head,*rear;
    TimeSlice *p,*q,*t;
    head=rear=nullptr;

    printf("Please input the number of process: ");
    scanf("%d",&numOfProcess);
    for (int i=0;i<numOfProcess;i++)
    {
        //init current process
        p=new TimeSlice();
        printf("Please input the name, arrive time and serve time of current process: ");
        scanf("%s%d%d",p->name,&p->arriveTime,&p->serveTime);
        p->remainTime=p->serveTime;
        p->state="Ready";
        p->next=nullptr;

        if (rear==nullptr)
        {
            head=p;
            p->next=nullptr;
            rear=p;
        }
        else
        {
            t=nullptr; q=head;
            while (q && q->arriveTime < p->arriveTime)
            {
                t=q;
                q=q->next;
            }
            if (q==head)
            {
                p->next=head;
                head=p;
            }
            else if (t==rear)
            {
                rear->next=p;
                p->next=nullptr;
                rear=p;
            }
            else
            {
                t->next=p;
                p->next=q;
            }
        }
    }
    return head;
}

void run(TimeSlice *head)
{
    TimeSlice *p,*t,*r;
    int num;
    vector<string>vec_out;
    printf("Please input the number of time slice: ");
    scanf("%d",&num);

    while (head!=nullptr)
    {
        r=p=head;
        while (p!=nullptr)
        {
            t=head;
            p->remainTime-=num;
            p->state="Running";
            string s=p->name;
            vec_out.pb(s);
            if (p->remainTime<0) p->remainTime=0;
            puts("\nthe program is running");
            puts("process  arriveTime  serveTime  remainTime  currentState");
            while (t!=nullptr)
            {
                printf("%3s%10d%10d%15d%16s\n",t->name,t->arriveTime,t->serveTime,t->remainTime,t->state);
                t=t->next;
            }
            if (p->remainTime==0)
            {
                if (p==head)
                {
                    head=p->next;
                    delete p;
                    p=head;
                }
                else
                {
                    r->next=p->next;
                    p=r->next;
                    r=p;
                }
            }
            else
            {
                r=p;
                p->state="Ready";
                p=p->next;
            }
        }
    }
    printf("load order: ");
    printf("%s",vec_out[0].c_str());
    for (int i=1;i<vec_out.size();i++)
        printf("-->%s ",vec_out[i].c_str());
}

void solve()
{
    TimeSlice *head=init();
    run(head);
}

int main()
{
    solve();
    return 0;
}
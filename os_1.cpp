// 151001215程虎威.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"stdio.h"
#include"stdlib.h"
#include"conio.h"
typedef struct pcb
{
	int name;
	bool state;
	int super;
	int time;
	struct pcb *next;
}PCB,*PCB_Link;
void show(PCB *p)
{
	PCB *q;
	q=p->next;
	while(q)
	{
		printf("进程名称: %d\t进程优先级: %d\t进程总时间: %d\n",q->name,q->super,q->time);
		q=q->next;
	}
}
void Insert(PCB_Link &Temp,PCB_Link mark)
{
	PCB_Link p;
	p=Temp;
	while(p->next)
	{
		p=p->next;
	}
	p->next=mark;
}
void FIFO(PCB_Link &ready,PCB_Link &wait)
{
	int TIME;
	PCB_Link p;
	p=ready->next;
	ready->next=p->next;
	p->next=NULL;
	TIME=rand()%50+1;
	printf("\n当前执行进程: %d\t进程优先级: %d\t进程总时间: %d\t CPU执行时间: %d\n",p->name,p->super,p->time,TIME);
	if(TIME>=p->time)
		printf("\nI Have Finished!\n\n");
	else
	{
		p->time-=TIME;
		int flag;
		flag=rand()%2;
		if(!flag)
			Insert(ready,p);
		else
			Insert(wait,p);
		printf("\nSorry!\n此时随机数为：%d\n",flag);
		
	}
		printf("就绪队列：\n");
		show(ready);
		printf("\n\n等待队列：\n\n");
		show(wait);



}
void priority(PCB_Link &ready,PCB_Link &wait)
{
	int TIME,temp=-1;
	PCB_Link p,mark;
	p=ready->next;
	while(p)
	{
		if(p->super>temp)
			{ 
				temp=p->super;
				mark=p;
			}
		p=p->next;
	}
	p=ready;
	while(p->next)
	{
		if(p->next==mark)
			{ 
				p->next=mark->next;
				mark->next=NULL;
				break;
			}
		p=p->next;
	}
	TIME=rand()%50+1;
	printf("\n当前执行进程: %d\t进程优先级: %d\t进程总时间: %d\t CPU执行时间: %d\n",mark->name,mark->super,mark->time,TIME);
	if(TIME>=mark->time)
		printf("\nI Have Finished!\n\n");
	else
	{
		mark->time-=TIME;
		int flag;
		flag=rand()%2;
		if(!flag)
			Insert(ready,mark);
		else
			Insert(wait,mark);
		printf("\nSorry!\n此时随机数为：%d\n",flag);
		
	}
		printf("就绪队列：\n");
		show(ready);
		printf("\n\n等待队列：\n\n");
		show(wait);

}
void min_time(PCB_Link &ready,PCB_Link &wait)
{
	int TIME,temp=100;
	PCB_Link p,mark;
	p=ready->next;
	while(p)
	{
		if(p->time<temp)
			{ 
				temp=p->time;
				mark=p;
			}
		p=p->next;
	}
	p=ready;
	while(p->next)
	{
		if(p->next==mark)
			{ 
				p->next=mark->next;
				mark->next=NULL;
				break;
			}
		p=p->next;
	}
	TIME=rand()%50+1;
	printf("\n当前执行进程: %d\t进程优先级: %d\t进程总时间: %d\t CPU执行时间: %d\n",mark->name,mark->super,mark->time,TIME);
	if(TIME>=mark->time)
		printf("\nI Have Finished!\n\n");
	else
	{
		mark->time-=TIME;
		int flag;
		flag=rand()%2;
		if(!flag)
			Insert(ready,mark);
		else
			Insert(wait,mark);
		printf("\nSorry!\n此时随机数为：%d\n",flag);
		
	}
		printf("就绪队列：\n");
		show(ready);
		printf("\n\n等待队列：\n\n");
		show(wait);

}
int _tmain(int argc, _TCHAR* argv[])
{
	PCB_Link ready,wait,r,w;
	ready=new PCB;
	ready->next=NULL;
	r=ready;
	wait=new PCB;
	wait->next=NULL;
	w=wait;
	int i;
	for(i=0;i<10;i++)
		{
			PCB *p;
			p=new PCB;
			p->name=i+1;
			p->time=rand()%50+1;
			p->next=NULL;
			p->state=rand()%2;
			p->super=rand()%10+1;
			if(p->state)
				{
					r->next=p;
					r=p;
				}
			else
			{
				w->next=p;
				w=p;
			}
		}
	PCB *a;
	a=ready->next;
	while(a)
	{
		//FIFO(ready,wait);
		//priority(ready,wait);
		min_time(ready,wait);
		a=ready->next;
	}


	system("pause");
	return 0;
	
}


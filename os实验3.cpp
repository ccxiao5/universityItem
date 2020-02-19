// 151001215程虎威.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"stdio.h"
#include"stdlib.h"
#include"string.h"
#include"math.h"
#include"algorithm"
#define L 1048576
using namespace std;
typedef struct Used_Node
{
	int add;
	int len;
	Used_Node *next;
}Used_Node,*Used_LinkList;
typedef struct Free_Node
{
	int add;
	int len;
	Free_Node *next;
}Free_Node,*Free_LinkList;
typedef struct PCB
{
	int memory;
	PCB *next;
}PCB,*PCB_LinkList;
void show(Used_LinkList used,Free_LinkList free)
{
	Used_LinkList p=used->next;
	printf("\n\n已分配表如下:\n");
	while(p)
	{
		printf("地址：%d\t\t\t长度：%d\n",p->add,p->len);
		p=p->next;
	}
	Free_LinkList q=free->next;
	printf("\n\n空闲表如下:\n");
	while(q)
	{
		printf("地址：%d\t\t\t长度：%d\n",q->add,q->len);
		q=q->next;
	}

}
void output(PCB_LinkList pcb)
{
	PCB *p=pcb->next;
	while(p)
	{
		printf("作业内存：%d\n",p->memory);
		p=p->next;
	}
}
void insert_Used(Used_LinkList &used,int add,int len)
{
	Used_LinkList t=used,p;
	p=new Used_Node;
	p->add=add;
	p->len=len;
	p->next=NULL;
	while(t->next){t=t->next;}
	t->next=p;
	t=p;
}
void insert_PCB(PCB_LinkList &pcb,PCB_LinkList p)
{
	PCB_LinkList t=pcb;
	while(t->next){t=t->next;}
	t->next=p;
	t=p;
	t->next=NULL;
}
void delete_Free(Free_LinkList &free,Free_LinkList p)
{
	Free_LinkList m=free,n=free->next;
	while(n)
	{
		if(n==p)
		{
			m->next=n->next;
			delete n;
			break;
		}
		else
		{
			m=m->next;
			n=n->next;
		}
	}
}
void delete_Used(Used_LinkList &used,Used_LinkList p)
{
	Used_LinkList m=used,n=used->next;
	while(n)
	{
		if(n==p)
		{
			m->next=n->next;
			delete n;
			break;
		}
		else
		{
			m=m->next;
			n=n->next;
		}
	}
}
void delete_PCB(PCB_LinkList &pcb,PCB_LinkList p)
{
	PCB_LinkList m=pcb,n=pcb->next;
	while(n)
	{
		if(n==p)
		{
			m->next=n->next;
			delete n;
			break;
		}
		else
		{
			m=m->next;
			n=n->next;
		}
	}
}
Free_LinkList search_BF(Free_LinkList free,PCB_LinkList p)
{
	int a[20],i,sum=0;
	Free_LinkList	t=free->next;
	while(t)
	{
		a[sum++]=t->len;
		t=t->next;
	}
	sort(a,a+sum);
	for(i=0;i<sum;i++)
		if(a[i]>p->memory)
			break;
	if(i==sum)
		return NULL;
	else
	{
		t=free->next;
		while(t)
		{
			if(t->len==a[i])
				break;
			t=t->next;
		}
		return t;
	}
}
Free_LinkList search_WF(Free_LinkList free,PCB_LinkList p)
{
		int a[20],i,sum=0;
	Free_LinkList	t=free->next;
	while(t)
	{
		a[sum++]=t->len;
		t=t->next;
	}
	sort(a,a+sum);
	for(i=sum-1;i>=0;i--)
		if(a[i]>p->memory)
			break;
	if(i==sum)
		return NULL;
	else
	{
		t=free->next;
		while(t)
		{
			if(t->len==a[i])
				break;
			t=t->next;
		}
		return t;
	}
}
void initialization(Used_LinkList &used,Free_LinkList &free,PCB_LinkList &pcb)
{
	Free_Node *p;
	PCB *t;
	used=new Used_Node;
	free=new Free_Node;
	pcb=new PCB;
	used->next=NULL;
	free->add=0;
	free->len=0;
	p=free;
	Free_Node *q;
	q=new Free_Node;
	q->add=p->add+p->len;
	q->len=L/pow(2,9.0);
	q->next=NULL;
	p->next=q;
	p=q;
	for(int i=1;i<10;i++)
	{
		Free_Node *q;
		q=new Free_Node;
		q->add=p->add+p->len;
		q->len=L/pow(2,double(10-i));
		q->next=NULL;
		p->next=q;
		p=q;
	}
		t=pcb;
		for(int i=1;i<=10;i++)
		{
			PCB *tt;
			tt=new PCB;
			tt->memory=rand()%L+1;
			tt->next=NULL;
			t->next=tt;
			t=tt;
		}
}
void FF(Used_LinkList &used,Free_LinkList &free,PCB_LinkList &pcb)
{
	Used_LinkList u;
	Free_LinkList f;
	PCB_LinkList p=pcb->next;
	int fflag;
	printf("\n\n首次适应算法~~~~~~~~\n");
	while(p)
	{
		f=free->next;
		while(f)
		{
			if(f->len>p->memory)
			{
				f->add+=p->memory;
				f->len-=p->memory;
				insert_Used(used,f->add-p->memory,p->memory);
				break;
			}
			else if(f->len==p->memory)
			{
				insert_Used(used,f->add,f->len);
				delete_Free(free,f);
				break;
			}
			else
				f=f->next;
		}
		if(!f){insert_PCB(pcb,p);}
		printf("------------------------------------------------\n");
		output(pcb);
		show(used,free);
		printf("***************************\n");
	//******************************************第一步
		u=used->next;
		if(u)//判断是否分配成功
		{
			int flag;
			flag=rand()%2;
			if(flag)
			{
				printf("flag=%d\n",flag);
				f=free->next;
				while(f)
				{
					if(f->add+f->len==u->add||u->add+u->len==f->add)
					{
						if(f->add+f->len==u->add&&u->add+u->len!=f->add)
						{
							f->len+=u->len;
							if(f->next&&u->add+u->len==f->next->add)
							{
								f->len+=f->next->len;
								delete_Free(free,f->next);
							}
						}
						else if(f->add+f->len!=u->add&&u->add+u->len==f->add)
						{
							f->add=u->add;
							f->len+=u->len;
						}
						fflag=1;
						delete_Used(used,u);
						break;
					}
					else{f=f->next;}
				}
			}
			else
			{
				f->add-=p->memory;
				f->len+=p->memory;
				fflag=0;
				delete_Used(used,u);
			}
			if(fflag)
				delete_PCB(pcb,p);
			p=pcb->next;
		}
		show(used,free);
	}

}
void BF(Used_LinkList &used,Free_LinkList &free,PCB_LinkList &pcb)
{
		Used_LinkList u;
		Free_LinkList f,temp;
		PCB_LinkList p=pcb->next;
		printf("\n\n最优适应算法~~~~~~~~\n");
		int fflag;
		while(p)
		{
			f=search_BF(free,p);
			if(!f){insert_PCB(pcb,p);}
			else
			{ 
				if(p->memory==f->len)
				{
					insert_Used(used,f->add,f->len);
					delete_Free(free,f);
				}
				else if(p->memory<f->len)
				{
					f->len-=p->memory;
					f->add+=p->memory;
					insert_Used(used,f->add-p->memory,p->memory);
				}
			}
			printf("------------------------------------------------\n");
			output(pcb);
			show(used,free);
			printf("***************************\n");
//******************************************第一步
			u=used->next;
			if(u)//判断是否分配成功
			{
				int flag;
				flag=rand()%2;
				printf("flag=%d\n",flag);
				if(flag)
				{			
					f=free->next;
					while(f)
					{
						if(f->add+f->len==u->add||u->add+u->len==f->add)
						{
							if(f->add+f->len==u->add&&u->add+u->len!=f->add)
							{
								f->len+=u->len;
								if(f->next&&u->add+u->len==f->next->add)
								{
									f->len+=f->next->len;
									delete_Free(free,f->next);
								}
							}
							else if(f->add+f->len!=u->add&&u->add+u->len==f->add)
							{
								f->add=u->add;
								f->len+=u->len;
							}
							fflag=1;
							delete_Used(used,u);
							break;
						}
						else{f=f->next;}
					}
				}
				else
				{
					f->add-=p->memory;
					f->len+=p->memory;
					fflag=0;
					delete_Used(used,u);
				}
				if(fflag)
					delete_PCB(pcb,p);
				p=pcb->next;
			}
			show(used,free);
		}
}
void WF(Used_LinkList &used,Free_LinkList &free,PCB_LinkList &pcb)
{
		Used_LinkList u;
		Free_LinkList f;
		PCB_LinkList p=pcb->next;
		printf("\n\n最差适应算法~~~~~~~~\n");
		int fflag;
		while(p)
		{
			f=search_WF(free,p);
			if(!f){insert_PCB(pcb,p);}
			else
			{ 
				if(p->memory==f->len)
				{
					insert_Used(used,f->add,f->len);
					delete_Free(free,f);
				}
				else if(p->memory<f->len)
				{
					f->len-=p->memory;
					f->add+=p->memory;
					insert_Used(used,f->add-p->memory,p->memory);
				}
			}
			printf("------------------------------------------------\n");
			output(pcb);
			show(used,free);
			printf("***************************\n");
//******************************************第一步
			u=used->next;
			if(u)//判断是否分配成功
			{
				int flag;
				flag=rand()%2;
				printf("flag=%d\n",flag);
				if(flag)
				{			
					f=free->next;
					while(f)
					{
						if(f->add+f->len==u->add||u->add+u->len==f->add)
						{
							if(f->add+f->len==u->add&&u->add+u->len!=f->add)
							{
								f->len+=u->len;
								if(f->next&&u->add+u->len==f->next->add)
								{
									f->len+=f->next->len;
									delete_Free(free,f->next);
								}
							}
							else if(f->add+f->len!=u->add&&u->add+u->len==f->add)
							{
								f->add=u->add;
								f->len+=u->len;
							}
							fflag=1;
							delete_Used(used,u);
							break;
						}
						else{f=f->next;}
					}
				}
				else
				{
					f->add-=p->memory;
					f->len+=p->memory;
					fflag=0;
					delete_Used(used,u);
				}
				if(fflag)
					delete_PCB(pcb,p);
				p=pcb->next;
			}
			show(used,free);
		}

}

int _tmain(int argc, _TCHAR* argv[])
{
	Used_Node *used;
	Free_Node *free;
	PCB *pcb;
	initialization(used,free,pcb);
	show(used,free);
	//FF(used,free,pcb);
	//BF(used,free,pcb);
	WF(used,free,pcb);
	system("pause");
	return 0;
	
}


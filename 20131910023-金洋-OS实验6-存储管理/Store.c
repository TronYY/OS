#include <stdio.h>
#include <windows.h>

#define FREE 0//空闲 /*表示每个物理块的状态*/
#define BUSY 1//占用
#define MAXLENGTH 100000//最大内存空间100000 Byte
#define MAXJOBNUM 1000//最大作业项数
#define MAXTIME 10000//最大时间
#define LEN sizeof(struct ElementType)




int JOBNUM;//作业个数
int CHOICE;//所选择的内存分配算法
int JOBIF[3][MAXJOBNUM];
/*
JOBIF[0][i]:第i项作业开始时间
JOBIF[1][i]:第i项作业执行时间
JOBIF[2][i]:第i项作业所需内存
**/



/*时间轴类型*/
struct TimeLine
{
	char time[MAXTIME];//记录0-9999这段时间内发生的分配内存'D'或回收内存'R'事件；
	int job_No[MAXTIME];//记录每个D或R对应的作业编号
};
struct TimeLine TLine;//时间轴


/*节点类型*/
struct ElementType
{
	int jobNo;//作业编号，作业名
	int statu;//显示状态
	int startAddress;//起始地址
	int size;//大小
	
	int startTime;//起始时间
	int runTime;//运行时间

	struct ElementType *pre;
	struct ElementType *next;
};
struct ElementType *HEAD,*TAIL;//分配链表的头结点




void input()
{
	int i;
	freopen("Store.in", "r", stdin);
	printf("这是金洋(Sno:20131910023)的并发进程存储管理模拟实验.\n");
	printf("1.最佳适应算法\n2.最差适应算法\n3.首次适应算法\n");
	printf("请输入内存分配算法:\n");
	scanf("%d",&CHOICE);

	printf("请输入作业个数:\n");
	scanf("%d",&JOBNUM);
	
	/*根据MATLAB产生的随机分布数填写每项进程信息*/
	printf("请依次输入n项作业的开始时间，执行时间，所需内存:\n");
	for (i=0;i<JOBNUM;i++)
	{
		scanf("%d %d %d",&JOBIF[0][i],&JOBIF[1][i],&JOBIF[2][i]);

	}

}





/*开辟一个带有头结点的分配链表*/
void newLink()
{
	HEAD=(struct ElementType*) malloc(LEN);
	TAIL=(struct ElementType*) malloc(LEN);
	
	TAIL->jobNo=-1;
	TAIL->statu=FREE;
	TAIL->startAddress=0;
	TAIL->size=MAXLENGTH;
	TAIL->pre=HEAD;
	TAIL->next=NULL;
	HEAD->next=TAIL;
	HEAD->pre=NULL;

}





/*最佳适应算法，为第num号作业分配内存*/
int bestFit(int num)
{
	int minRest=MAXLENGTH;//最小剩余空间
	struct ElementType *p=HEAD->next;//工作指针
	struct ElementType *q=NULL;//记录最佳插入位置

	struct ElementType *temp;//为第num号作业单元
	temp=(struct ElementType*) malloc(LEN);
	temp->jobNo=num;
	temp->statu=BUSY;
	temp->size=JOBIF[2][num];





	/*遍历整个分配链表*/
	while (p)
	{
		/*找到一个合适的分区*/
		if (p->statu==FREE && p->size>=temp->size)
		{
			/* q还是刚初始化 */  /* q已经被修改，但是不是最佳的*/
			if ((q==NULL) || (p->size < q->size))
			{
				q=p;
				minRest=p->size - temp->size; 

			}
		}
		p=p->next;
	}

	/*没有找到合适的物理块，分配失败*/
	if (q==NULL) return 0;
	


	/*最佳插入位置的大小刚好等于所需大小*/
	if (q->size==temp->size)
	{
		q->jobNo=num;
		q->statu=BUSY;
		return 1;
	}
	/*最佳插入位置的大小  大于 所需大小*/
	else
	{
		temp->startAddress=q->startAddress;
		temp->pre=q->pre;
		temp->next=q;

		temp->pre->next=temp;

		q->pre=temp;
		q->startAddress+=temp->size;
		q->size=minRest;//修改剩余大小
		return 1;
	}
}




/*最差适应算法，为第num号作业分配内存*/
int worstFit(int num)
{
	

	int maxRest=-1;//最大剩余空间
	struct ElementType *p=HEAD->next;//工作指针
	struct ElementType *q=NULL;//记录最佳插入位置

	struct ElementType *temp;//为第num号作业单元
	temp=(struct ElementType*) malloc(LEN);
	temp->jobNo=num;
	temp->statu=BUSY;
	temp->size=JOBIF[2][num];


	


	/*遍历整个分配链表*/
	while (p)
	{
		/*找到一个合适的分区*/
		if (p->statu==FREE && p->size>=temp->size)
		{
			/* q还是刚初始化 */  /* q已经被修改，但是不是最佳的*/
			if ((q==NULL) || (p->size > q->size))
			{
				q=p;
				maxRest=p->size - temp->size; 

			}
		}
		p=p->next;
	}

	/*没有找到合适的物理块，分配失败*/
	if (q==NULL) return 0;
	


	/*最佳插入位置的大小刚好等于所需大小*/
	if (q->size==temp->size)
	{
		q->jobNo=num;
		q->statu=BUSY;
		return 1;
	}
	/*最佳插入位置的大小  大于 所需大小*/
	else
	{
		temp->startAddress=q->startAddress;
		temp->pre=q->pre;
		temp->next=q;

		temp->pre->next=temp;

		q->pre=temp;
		q->startAddress+=temp->size;
		q->size=maxRest;//修改剩余大小
		return 1;
	}
}




/*首次适应算法，为第num号作业分配内存*/
int firstFit(int num)
{
	struct ElementType *p=HEAD->next;//工作指针
	struct ElementType *q=NULL;//记录最佳插入位置

	struct ElementType *temp;//为第num号作业单元
	temp=(struct ElementType*) malloc(LEN);
	temp->jobNo=num;
	temp->statu=BUSY;
	temp->size=JOBIF[2][num];


	


	/*遍历整个分配链表*/
	while (p)
	{
		/*找到一个合适的分区*/
		if (p->statu==FREE && p->size>=temp->size)
			break;
		p=p->next;
	}
	q=p;

	
	/*没有找到合适的物理块，分配失败*/
	if (q==NULL) return 0;
	


	/*插入位置的大小刚好等于所需大小*/
	if (q->size==temp->size)
	{
		q->jobNo=num;
		q->statu=BUSY;
		return 1;
	}
	/*最佳插入位置的大小  大于 所需大小*/
	else
	{
		temp->startAddress=q->startAddress;
		temp->pre=q->pre;
		temp->next=q;

		temp->pre->next=temp;

		q->pre=temp;
		q->startAddress+=temp->size;
		q->size-=temp->size;//**修改剩余大小
		return 1;
	}
}







/*回收内存，回收第num号作业所占的内存*/
int reclaim(int num)
{
	struct ElementType *p=HEAD->next;//工作指针

	if (num<0 || num>=JOBNUM) return 0;

	

	/*遍历分配链表*/
	while (p)
	{
		if (p->jobNo==num && p->statu==BUSY) break;
		p=p->next;
	}

	/*遍历后没有发现第num号作业所占的内存,回收内存失败*/
	if (p==NULL) return 0;
	
	p->statu=FREE;

	/*回收区与其前一空闲分区F1相邻接，此时应将回收区与F1合并，修改F1的大小*/
	if (p->pre!=HEAD && p->pre->statu==FREE)
	{
		p->pre->size+=p->size;
		p->pre->next=p->next;
		p->next->pre=p->pre;

		p=p->pre;
	}


	/*回收区与其后一空闲分区F2相邻接，此时应将回收区与F2合并，修改回收区的大小，并取消F2的表项*/
	/*后一分区不是尾节点*/
	if (p->next!=TAIL && p->next->statu==FREE)
	{
		p->size+=p->next->size;
		p->next=p->next->next;
		free(p->next->pre);//free(F2)相当于取消F2的表项
		p->next->pre=p;
	}


	/*后一分区是尾节点*/
	if (p->next==TAIL && p->next->statu==FREE)
	{
		p->size+=p->next->size;
		p->next=NULL;
		TAIL=p;
	}

	return 1;
}



void showMemory()
{
	struct ElementType *p=HEAD->next;//工作指针
	int partNum=0;//分区号

	printf("内存分配情况:\n");
	printf("分区号\t起始地址\t分区大小\t状态\t作业号\n");

	
	while (p)
	{
		printf("  %d\t",partNum++);
		printf("  %d\t\t",p->startAddress);
		printf(" %d\t\t",p->size);
		
		if (p->statu==FREE)
			printf("空闲\t  -\n");
		else 
			printf("占用\t  %d\n",p->jobNo);
		p=p->next;


	}
	printf("\n\n\n");
	

}









/*可以在此处决定调度模式，此处沿时间轴遍历，即先来先服务调度*/
void play()
{
	int flag;//分配、回收是否成功

	/*创建时间轴*/
	int i,startT=9999999,endT=-1;//最早开始时间和最迟结束时间
	
	int s,e;//记录每一项作业的开始时间和结束时间；
	for (i=0;i<JOBNUM;i++)
	{
		s=JOBIF[0][i];
		/*在时间轴上标记第i项作业开始时间*/
		while (TLine.time[s]=='S' || TLine.time[s]=='E') 
		{
			s++;//此时已经有事件，则推后本事件
		}
		TLine.time[s]='S';
		TLine.job_No[s]=i;
	
		/*在时间轴上标记第i项作业结束时间*/
		e=JOBIF[1][i]+s;
		while (TLine.time[e]=='S' || TLine.time[e]=='E') 
		{
			e++;
		}
		TLine.time[e]='E';
		TLine.job_No[e]=i;

		if (s<startT) startT=s;
		if (e>endT) endT=e;
	}


	/*开辟一个带有头结点的分配链表*/
	newLink();

	
	


	/*沿时间轴遍历*/
	/*即先来先服务调度*/
	printf("\n");
	for (i=0;i<=endT;i++)
	{
	
		printf("第%d时间单位:\n",i);
		if (TLine.time[i]=='S')
		{
			switch(CHOICE)
			{
				case 1:flag=bestFit(TLine.job_No[i]);break;
				case 2:flag=worstFit(TLine.job_No[i]);break;
				case 3:flag=firstFit(TLine.job_No[i]);break;
			}
			
			printf("为第%d项作业",TLine.job_No[i]);
			if (flag==1) printf("分配内存成功\n");
			else printf("分配内存失败\n");
		}


		else if (TLine.time[i]=='E')
		{
			flag=reclaim(TLine.job_No[i]);
			if (flag==1) printf("回收第%d项作业内存成功\n",TLine.job_No[i]);
			else printf("回收第%d项作业内存失败\n",TLine.job_No[i]);
		}
		
		/*每一秒后显示内存状态*/
		showMemory();
		//Sleep(1000);
	}
}






void main()
{
	input();
	play();



}
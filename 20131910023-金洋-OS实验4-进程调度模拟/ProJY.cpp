#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define UNREADY 0 //未就绪
#define READY 1 //就绪
#define RUN 2 //运行
#define WAIT 3 //阻塞
#define LEN sizeof(struct PCB)
#define TIMER 5//每个时间片是5个单位时间

/*定义进程结构体*/
struct PCB
{
	int pID;//PID
	char name[10];
	int statue;//进程状态
	int needTime;//所需执行时间(以1为单位，下同)
	int waitTime;//等待时间
	int initialPriority;//初始优先级
	int currentPriority;//当前优先级
	struct PCB *next;
};

struct PCB *HEAD,*TAIL;
int PRONUM=0,WATCH=0;//总进程数
char *STATUE[]={"UNREADY","READY","RUN","WAIT"};





void printAuthor()
{
	printf("这是金洋(Sno:20131910023)的进程调度模拟实验.\n");
}





/*进程信息输入*/
void create()
{
	int num,i;

	printf("请输入你想建立的进程个数:");
	scanf("%d",&num);
	

	for (i=0;i<num;i++)
	{
		
		//工作指针
		struct PCB *P;
		P=(struct PCB*) malloc(LEN);
		PRONUM++;//更新总进程数
		printf("\n请输入第%d个进程信息:\n",PRONUM);
		P->pID=PRONUM;
		printf("请输入进程名:");
		scanf("%s",&P->name);
		P->statue=UNREADY;//初始时都是未就绪状态
		printf("请输入进程所需执行时间:");
		scanf("%d",&P->needTime);
		P->waitTime=0;
		printf("请输入进程优先级:");
		scanf("%d",&P->initialPriority);
		P->currentPriority=P->initialPriority;//初始时，当前优先级=初始优先级
		P->next=NULL;

		/*将新进程放到队列尾部*/
		TAIL->next=P;
		TAIL=P;
		
	}

}





/*显示已创建的进程*/
void current()
{

	int i;
	struct PCB *P;
	P=HEAD->next;
	if (P==NULL) 
	{
		printf("就绪队列无进程,无法显示进程信息.");
		return;
	}

	for (i=0;i<PRONUM;i++)
	{
		printf("PID:%d\n",P->pID);
		printf("进程名:%s\n",P->name);
		printf("进程状态:%s\n",STATUE[P->statue]);
		printf("\n");
		P=P->next;
	}
}







/*1、一个进程在就绪对列中等待了若干个时间单位（如15个），则将它的当前优先级加1。
2、若当前运行的进程时间片到，则中止其运行（抢占式多任务），将其放入就绪队列中，它的当前优先级也恢复为初始优先级*/


/*优先级调度进程方式*/
void priModel()
{
	struct PCB *P,*PRun;//P为工作指针，PRun为指向优先级最高的进程
	


	while (PRONUM>0)
	{
		P=PRun=HEAD;
		if (P==NULL) 
		{
			printf("就绪队列无进程,无法调度.");
			return;
		}




		/*寻找优先级最高的进程*/
		while (P->next!=NULL)
		{
		
			P=P->next;
			/*寻找优先级最高的进程*/
			if (P->currentPriority > PRun->currentPriority)
				PRun=P;
			else
			{
				/* 一个进程在就绪对列中等待了15个单位时间，则将它的当前优先级加1 */
				/*实际上应该在时间片运转之后加，此处为了方便和节约时间，在寻找优先级最高进程的过程中提前加了优先级，效果是一样的*/
			
				P->waitTime+=TIMER;//一个优先级最高的进程被调度了，意味着其他进程都要等待TIMER个时间
				if (P->waitTime%15==0)
					P->currentPriority++;
			}
		}

		



		
		/*PRun所指的进程被调度运行*/
		int i;
		PRun->statue=RUN;/*先将状态改变*/
		for (i=0;i<TIMER;i++)
		{
			Sleep(1000);
			printf("\n");
			printf("第%d单位时间:\n",++WATCH);

			PRun->needTime--;
			if (PRun->needTime>=0)
			/*PRun所指的进程还未运行完*/
			{
				printf("PID:%d\n",PRun->pID);
				printf("进程名:%s\n",PRun->name);
				printf("进程状态:%s\n",STATUE[PRun->statue]);
				printf("剩余执行时间:%d\n",PRun->needTime);
				printf("当前优先级:%d\n",PRun->currentPriority);
			}
			else 
				printf("无进程运行\n");
		}
		
		


		/*PRun所指的进程是否运行完*/
		if (PRun->needTime<=0)
		{
			/*运行完时，删除该节点来模拟撤销进程*/
			P=HEAD;
			while (P->next!=PRun)
				P=P->next;
			if (PRun==TAIL)
			{
				TAIL=P;
				TAIL->next=NULL;
			}
			else
			{
				P->next=PRun->next;
				PRun->next=NULL;
			}
			free(PRun);
			PRONUM--;
		}

		/*进程尚未运行完*/
		else

		{
			PRun->statue=WAIT;//状态设为阻塞
			PRun->waitTime=0;//等待时间清零
			PRun->currentPriority=PRun->initialPriority;//优先级设为初始优先级
		}
			
	}
}




void main()
{
	printAuthor();
	int inputOfUser;

	HEAD=(struct PCB*) malloc(LEN);//开辟一个带有头结点链表单元
	HEAD->next=NULL;
	HEAD->currentPriority=-99999;
	TAIL=HEAD;


	while (1)
	{
		printf("\n主菜单：\n");
		printf("1:创建进程\n");
		printf("2:显示当前运行进程和就绪队列中进程信息\n");
		printf("3:优先级调度\n");
		printf("4:退出系统\n");
		printf("\n请选择:");
		

		scanf("%d",&inputOfUser);
		switch (inputOfUser)
		{
			case 1: create();     //创建进程
					break; 
			case 2: current();     //显示已创建的进程
					break; 
			case 3: priModel();    //优先级调度进程方式
					break;
			case 4: exit(0); 


		}



	}



}

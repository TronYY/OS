#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#define UNREADY 0 //δ����
#define READY 1 //����
#define RUN 2 //����
#define WAIT 3 //����
#define LEN sizeof(struct PCB)
#define TIMER 5//ÿ��ʱ��Ƭ��5����λʱ��

/*������̽ṹ��*/
struct PCB
{
	int pID;//PID
	char name[10];
	int statue;//����״̬
	int needTime;//����ִ��ʱ��(��1Ϊ��λ����ͬ)
	int waitTime;//�ȴ�ʱ��
	int initialPriority;//��ʼ���ȼ�
	int currentPriority;//��ǰ���ȼ�
	struct PCB *next;
};

struct PCB *HEAD,*TAIL;
int PRONUM=0,WATCH=0;//�ܽ�����
char *STATUE[]={"UNREADY","READY","RUN","WAIT"};





void printAuthor()
{
	printf("���ǽ���(Sno:20131910023)�Ľ��̵���ģ��ʵ��.\n");
}





/*������Ϣ����*/
void create()
{
	int num,i;

	printf("���������뽨���Ľ��̸���:");
	scanf("%d",&num);
	

	for (i=0;i<num;i++)
	{
		
		//����ָ��
		struct PCB *P;
		P=(struct PCB*) malloc(LEN);
		PRONUM++;//�����ܽ�����
		printf("\n�������%d��������Ϣ:\n",PRONUM);
		P->pID=PRONUM;
		printf("�����������:");
		scanf("%s",&P->name);
		P->statue=UNREADY;//��ʼʱ����δ����״̬
		printf("�������������ִ��ʱ��:");
		scanf("%d",&P->needTime);
		P->waitTime=0;
		printf("������������ȼ�:");
		scanf("%d",&P->initialPriority);
		P->currentPriority=P->initialPriority;//��ʼʱ����ǰ���ȼ�=��ʼ���ȼ�
		P->next=NULL;

		/*���½��̷ŵ�����β��*/
		TAIL->next=P;
		TAIL=P;
		
	}

}





/*��ʾ�Ѵ����Ľ���*/
void current()
{

	int i;
	struct PCB *P;
	P=HEAD->next;
	if (P==NULL) 
	{
		printf("���������޽���,�޷���ʾ������Ϣ.");
		return;
	}

	for (i=0;i<PRONUM;i++)
	{
		printf("PID:%d\n",P->pID);
		printf("������:%s\n",P->name);
		printf("����״̬:%s\n",STATUE[P->statue]);
		printf("\n");
		P=P->next;
	}
}







/*1��һ�������ھ��������еȴ������ɸ�ʱ�䵥λ����15�����������ĵ�ǰ���ȼ���1��
2������ǰ���еĽ���ʱ��Ƭ��������ֹ�����У���ռʽ�����񣩣����������������У����ĵ�ǰ���ȼ�Ҳ�ָ�Ϊ��ʼ���ȼ�*/


/*���ȼ����Ƚ��̷�ʽ*/
void priModel()
{
	struct PCB *P,*PRun;//PΪ����ָ�룬PRunΪָ�����ȼ���ߵĽ���
	


	while (PRONUM>0)
	{
		P=PRun=HEAD;
		if (P==NULL) 
		{
			printf("���������޽���,�޷�����.");
			return;
		}




		/*Ѱ�����ȼ���ߵĽ���*/
		while (P->next!=NULL)
		{
		
			P=P->next;
			/*Ѱ�����ȼ���ߵĽ���*/
			if (P->currentPriority > PRun->currentPriority)
				PRun=P;
			else
			{
				/* һ�������ھ��������еȴ���15����λʱ�䣬�����ĵ�ǰ���ȼ���1 */
				/*ʵ����Ӧ����ʱ��Ƭ��ת֮��ӣ��˴�Ϊ�˷���ͽ�Լʱ�䣬��Ѱ�����ȼ���߽��̵Ĺ�������ǰ�������ȼ���Ч����һ����*/
			
				P->waitTime+=TIMER;//һ�����ȼ���ߵĽ��̱������ˣ���ζ���������̶�Ҫ�ȴ�TIMER��ʱ��
				if (P->waitTime%15==0)
					P->currentPriority++;
			}
		}

		



		
		/*PRun��ָ�Ľ��̱���������*/
		int i;
		PRun->statue=RUN;/*�Ƚ�״̬�ı�*/
		for (i=0;i<TIMER;i++)
		{
			Sleep(1000);
			printf("\n");
			printf("��%d��λʱ��:\n",++WATCH);

			PRun->needTime--;
			if (PRun->needTime>=0)
			/*PRun��ָ�Ľ��̻�δ������*/
			{
				printf("PID:%d\n",PRun->pID);
				printf("������:%s\n",PRun->name);
				printf("����״̬:%s\n",STATUE[PRun->statue]);
				printf("ʣ��ִ��ʱ��:%d\n",PRun->needTime);
				printf("��ǰ���ȼ�:%d\n",PRun->currentPriority);
			}
			else 
				printf("�޽�������\n");
		}
		
		


		/*PRun��ָ�Ľ����Ƿ�������*/
		if (PRun->needTime<=0)
		{
			/*������ʱ��ɾ���ýڵ���ģ�⳷������*/
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

		/*������δ������*/
		else

		{
			PRun->statue=WAIT;//״̬��Ϊ����
			PRun->waitTime=0;//�ȴ�ʱ������
			PRun->currentPriority=PRun->initialPriority;//���ȼ���Ϊ��ʼ���ȼ�
		}
			
	}
}




void main()
{
	printAuthor();
	int inputOfUser;

	HEAD=(struct PCB*) malloc(LEN);//����һ������ͷ�������Ԫ
	HEAD->next=NULL;
	HEAD->currentPriority=-99999;
	TAIL=HEAD;


	while (1)
	{
		printf("\n���˵���\n");
		printf("1:��������\n");
		printf("2:��ʾ��ǰ���н��̺;��������н�����Ϣ\n");
		printf("3:���ȼ�����\n");
		printf("4:�˳�ϵͳ\n");
		printf("\n��ѡ��:");
		

		scanf("%d",&inputOfUser);
		switch (inputOfUser)
		{
			case 1: create();     //��������
					break; 
			case 2: current();     //��ʾ�Ѵ����Ľ���
					break; 
			case 3: priModel();    //���ȼ����Ƚ��̷�ʽ
					break;
			case 4: exit(0); 


		}



	}



}

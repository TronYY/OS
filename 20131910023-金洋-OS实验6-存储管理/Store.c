#include <stdio.h>
#include <windows.h>

#define FREE 0//���� /*��ʾÿ��������״̬*/
#define BUSY 1//ռ��
#define MAXLENGTH 100000//����ڴ�ռ�100000 Byte
#define MAXJOBNUM 1000//�����ҵ����
#define MAXTIME 10000//���ʱ��
#define LEN sizeof(struct ElementType)




int JOBNUM;//��ҵ����
int CHOICE;//��ѡ����ڴ�����㷨
int JOBIF[3][MAXJOBNUM];
/*
JOBIF[0][i]:��i����ҵ��ʼʱ��
JOBIF[1][i]:��i����ҵִ��ʱ��
JOBIF[2][i]:��i����ҵ�����ڴ�
**/



/*ʱ��������*/
struct TimeLine
{
	char time[MAXTIME];//��¼0-9999���ʱ���ڷ����ķ����ڴ�'D'������ڴ�'R'�¼���
	int job_No[MAXTIME];//��¼ÿ��D��R��Ӧ����ҵ���
};
struct TimeLine TLine;//ʱ����


/*�ڵ�����*/
struct ElementType
{
	int jobNo;//��ҵ��ţ���ҵ��
	int statu;//��ʾ״̬
	int startAddress;//��ʼ��ַ
	int size;//��С
	
	int startTime;//��ʼʱ��
	int runTime;//����ʱ��

	struct ElementType *pre;
	struct ElementType *next;
};
struct ElementType *HEAD,*TAIL;//���������ͷ���




void input()
{
	int i;
	freopen("Store.in", "r", stdin);
	printf("���ǽ���(Sno:20131910023)�Ĳ������̴洢����ģ��ʵ��.\n");
	printf("1.�����Ӧ�㷨\n2.�����Ӧ�㷨\n3.�״���Ӧ�㷨\n");
	printf("�������ڴ�����㷨:\n");
	scanf("%d",&CHOICE);

	printf("��������ҵ����:\n");
	scanf("%d",&JOBNUM);
	
	/*����MATLAB����������ֲ�����дÿ�������Ϣ*/
	printf("����������n����ҵ�Ŀ�ʼʱ�䣬ִ��ʱ�䣬�����ڴ�:\n");
	for (i=0;i<JOBNUM;i++)
	{
		scanf("%d %d %d",&JOBIF[0][i],&JOBIF[1][i],&JOBIF[2][i]);

	}

}





/*����һ������ͷ���ķ�������*/
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





/*�����Ӧ�㷨��Ϊ��num����ҵ�����ڴ�*/
int bestFit(int num)
{
	int minRest=MAXLENGTH;//��Сʣ��ռ�
	struct ElementType *p=HEAD->next;//����ָ��
	struct ElementType *q=NULL;//��¼��Ѳ���λ��

	struct ElementType *temp;//Ϊ��num����ҵ��Ԫ
	temp=(struct ElementType*) malloc(LEN);
	temp->jobNo=num;
	temp->statu=BUSY;
	temp->size=JOBIF[2][num];





	/*����������������*/
	while (p)
	{
		/*�ҵ�һ�����ʵķ���*/
		if (p->statu==FREE && p->size>=temp->size)
		{
			/* q���Ǹճ�ʼ�� */  /* q�Ѿ����޸ģ����ǲ�����ѵ�*/
			if ((q==NULL) || (p->size < q->size))
			{
				q=p;
				minRest=p->size - temp->size; 

			}
		}
		p=p->next;
	}

	/*û���ҵ����ʵ�����飬����ʧ��*/
	if (q==NULL) return 0;
	


	/*��Ѳ���λ�õĴ�С�պõ��������С*/
	if (q->size==temp->size)
	{
		q->jobNo=num;
		q->statu=BUSY;
		return 1;
	}
	/*��Ѳ���λ�õĴ�С  ���� �����С*/
	else
	{
		temp->startAddress=q->startAddress;
		temp->pre=q->pre;
		temp->next=q;

		temp->pre->next=temp;

		q->pre=temp;
		q->startAddress+=temp->size;
		q->size=minRest;//�޸�ʣ���С
		return 1;
	}
}




/*�����Ӧ�㷨��Ϊ��num����ҵ�����ڴ�*/
int worstFit(int num)
{
	

	int maxRest=-1;//���ʣ��ռ�
	struct ElementType *p=HEAD->next;//����ָ��
	struct ElementType *q=NULL;//��¼��Ѳ���λ��

	struct ElementType *temp;//Ϊ��num����ҵ��Ԫ
	temp=(struct ElementType*) malloc(LEN);
	temp->jobNo=num;
	temp->statu=BUSY;
	temp->size=JOBIF[2][num];


	


	/*����������������*/
	while (p)
	{
		/*�ҵ�һ�����ʵķ���*/
		if (p->statu==FREE && p->size>=temp->size)
		{
			/* q���Ǹճ�ʼ�� */  /* q�Ѿ����޸ģ����ǲ�����ѵ�*/
			if ((q==NULL) || (p->size > q->size))
			{
				q=p;
				maxRest=p->size - temp->size; 

			}
		}
		p=p->next;
	}

	/*û���ҵ����ʵ�����飬����ʧ��*/
	if (q==NULL) return 0;
	


	/*��Ѳ���λ�õĴ�С�պõ��������С*/
	if (q->size==temp->size)
	{
		q->jobNo=num;
		q->statu=BUSY;
		return 1;
	}
	/*��Ѳ���λ�õĴ�С  ���� �����С*/
	else
	{
		temp->startAddress=q->startAddress;
		temp->pre=q->pre;
		temp->next=q;

		temp->pre->next=temp;

		q->pre=temp;
		q->startAddress+=temp->size;
		q->size=maxRest;//�޸�ʣ���С
		return 1;
	}
}




/*�״���Ӧ�㷨��Ϊ��num����ҵ�����ڴ�*/
int firstFit(int num)
{
	struct ElementType *p=HEAD->next;//����ָ��
	struct ElementType *q=NULL;//��¼��Ѳ���λ��

	struct ElementType *temp;//Ϊ��num����ҵ��Ԫ
	temp=(struct ElementType*) malloc(LEN);
	temp->jobNo=num;
	temp->statu=BUSY;
	temp->size=JOBIF[2][num];


	


	/*����������������*/
	while (p)
	{
		/*�ҵ�һ�����ʵķ���*/
		if (p->statu==FREE && p->size>=temp->size)
			break;
		p=p->next;
	}
	q=p;

	
	/*û���ҵ����ʵ�����飬����ʧ��*/
	if (q==NULL) return 0;
	


	/*����λ�õĴ�С�պõ��������С*/
	if (q->size==temp->size)
	{
		q->jobNo=num;
		q->statu=BUSY;
		return 1;
	}
	/*��Ѳ���λ�õĴ�С  ���� �����С*/
	else
	{
		temp->startAddress=q->startAddress;
		temp->pre=q->pre;
		temp->next=q;

		temp->pre->next=temp;

		q->pre=temp;
		q->startAddress+=temp->size;
		q->size-=temp->size;//**�޸�ʣ���С
		return 1;
	}
}







/*�����ڴ棬���յ�num����ҵ��ռ���ڴ�*/
int reclaim(int num)
{
	struct ElementType *p=HEAD->next;//����ָ��

	if (num<0 || num>=JOBNUM) return 0;

	

	/*������������*/
	while (p)
	{
		if (p->jobNo==num && p->statu==BUSY) break;
		p=p->next;
	}

	/*������û�з��ֵ�num����ҵ��ռ���ڴ�,�����ڴ�ʧ��*/
	if (p==NULL) return 0;
	
	p->statu=FREE;

	/*����������ǰһ���з���F1���ڽӣ���ʱӦ����������F1�ϲ����޸�F1�Ĵ�С*/
	if (p->pre!=HEAD && p->pre->statu==FREE)
	{
		p->pre->size+=p->size;
		p->pre->next=p->next;
		p->next->pre=p->pre;

		p=p->pre;
	}


	/*�����������һ���з���F2���ڽӣ���ʱӦ����������F2�ϲ����޸Ļ������Ĵ�С����ȡ��F2�ı���*/
	/*��һ��������β�ڵ�*/
	if (p->next!=TAIL && p->next->statu==FREE)
	{
		p->size+=p->next->size;
		p->next=p->next->next;
		free(p->next->pre);//free(F2)�൱��ȡ��F2�ı���
		p->next->pre=p;
	}


	/*��һ������β�ڵ�*/
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
	struct ElementType *p=HEAD->next;//����ָ��
	int partNum=0;//������

	printf("�ڴ�������:\n");
	printf("������\t��ʼ��ַ\t������С\t״̬\t��ҵ��\n");

	
	while (p)
	{
		printf("  %d\t",partNum++);
		printf("  %d\t\t",p->startAddress);
		printf(" %d\t\t",p->size);
		
		if (p->statu==FREE)
			printf("����\t  -\n");
		else 
			printf("ռ��\t  %d\n",p->jobNo);
		p=p->next;


	}
	printf("\n\n\n");
	

}









/*�����ڴ˴���������ģʽ���˴���ʱ����������������ȷ������*/
void play()
{
	int flag;//���䡢�����Ƿ�ɹ�

	/*����ʱ����*/
	int i,startT=9999999,endT=-1;//���翪ʼʱ�����ٽ���ʱ��
	
	int s,e;//��¼ÿһ����ҵ�Ŀ�ʼʱ��ͽ���ʱ�䣻
	for (i=0;i<JOBNUM;i++)
	{
		s=JOBIF[0][i];
		/*��ʱ�����ϱ�ǵ�i����ҵ��ʼʱ��*/
		while (TLine.time[s]=='S' || TLine.time[s]=='E') 
		{
			s++;//��ʱ�Ѿ����¼������ƺ��¼�
		}
		TLine.time[s]='S';
		TLine.job_No[s]=i;
	
		/*��ʱ�����ϱ�ǵ�i����ҵ����ʱ��*/
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


	/*����һ������ͷ���ķ�������*/
	newLink();

	
	


	/*��ʱ�������*/
	/*�������ȷ������*/
	printf("\n");
	for (i=0;i<=endT;i++)
	{
	
		printf("��%dʱ�䵥λ:\n",i);
		if (TLine.time[i]=='S')
		{
			switch(CHOICE)
			{
				case 1:flag=bestFit(TLine.job_No[i]);break;
				case 2:flag=worstFit(TLine.job_No[i]);break;
				case 3:flag=firstFit(TLine.job_No[i]);break;
			}
			
			printf("Ϊ��%d����ҵ",TLine.job_No[i]);
			if (flag==1) printf("�����ڴ�ɹ�\n");
			else printf("�����ڴ�ʧ��\n");
		}


		else if (TLine.time[i]=='E')
		{
			flag=reclaim(TLine.job_No[i]);
			if (flag==1) printf("���յ�%d����ҵ�ڴ�ɹ�\n",TLine.job_No[i]);
			else printf("���յ�%d����ҵ�ڴ�ʧ��\n",TLine.job_No[i]);
		}
		
		/*ÿһ�����ʾ�ڴ�״̬*/
		showMemory();
		//Sleep(1000);
	}
}






void main()
{
	input();
	play();



}
#include <iostream>
#include <windows.h>
#include <math.h>

/*ͨ��Prim�㷨��ʵ���ж�һ�������Ȩͼ�Ƿ���ͨ������ͨ��������һ����С֧������*/
/*����ԭ����㷨����*/
#define maxnum 999999999
int X[100],F[2][100],Fnum,sum,n=8;

/*M[1*n,1*n]Ϊ�����Ȩͼ*/
/*�˴�Ϊ�˼������빤��������ǰ��Mָ��*/
int M[9][9]={

	{0,0,0,0,0,0,0,0,0},
	{0,0,3,2,0,0,0,0,0},
	{0,3,0,3,6,0,0,0,0},
	{0,2,3,0,0,5,5,0,0},
	{0,0,6,0,0,2,0,0,2},
	{0,0,0,5,2,0,4,3,0},
	{0,0,0,5,0,4,0,4,0},
	{0,0,0,0,0,3,4,0,6},
	{0,0,0,0,2,0,0,6,0}
	  
	};


int equal()
{
	int i;
	for(i=1;i<=n;i++)
		if (X[i]==0) return(0);
	return(1);
}



int dota()
{
	int i,j;
	for(i=1;i<=n;i++)
		for (j=1;j<=n;j++)
			if ((i!=j)&&(X[i])&&(X[j]==0)&&(M[i][j]<maxnum)) return(1);
	return(0);
}


void prim()
{
	int i,j,min,v1=0,v2=0;
	min=maxnum;
	for(i=1;i<=n;i++)
		if (X[i])

		for(j=1;j<=n;j++)
			if ((!X[j])&&(i!=j))

				if (M[i][j]<min)
				{
					min=M[i][j];
					v1=i;
					v2=j;
				}
	if (min!=maxnum)
	{
		X[v2]=1;
		Fnum++;
		F[0][Fnum]=v1;
		F[1][Fnum]=v2;
		sum=sum+min;
	}
}

/*�����������㷨������*/
DWORD WINAPI primMain(LPVOID param)
{  
	int i,j;
	
	

	for (i=1;i<=n;i++)
	   for (j=1;j<=n;j++)
	   {
		   
		   if (M[i][j]==0) M[i][j]=maxnum;
	   }
	  
	for (i=1;i<=n;i++) X[i]=0;
	X[1]=1;


	while ((!equal())&&(dota())) prim();


	if (equal())
	{
	   for(i=1;i<=Fnum;i++)
		   printf("%c,%c\n",F[0][i]+64,F[1][i]+64);
	   printf("The total weight is %d.",sum);
	}
	else printf("G is not connected.");

	return 0;
}








void main()
{
	HANDLE timeThread,primerThread,MSTThread;//���������߳�
	DWORD id1,id2,id3;//�ֱ��ʾ�����̵߳�id

	/*���´��������̣߳��Ҵ������Ȳ�����ִ�ж���*/

	/*��ȡ��ǰ��������ʱ���߳�*/

	MSTThread=CreateThread(NULL,0,primMain,NULL,0,&id3);


}
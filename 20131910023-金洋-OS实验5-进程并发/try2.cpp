#include <iostream>
#include <windows.h>
#include <math.h>

/*通过Prim算法，实现判断一个无向加权图是否连通，若连通，则求其一种最小支撑树；*/
/*程序原理见算法介绍*/
#define maxnum 999999999
int X[100],F[2][100],Fnum,sum,n=8;

/*M[1*n,1*n]为无向加权图*/
/*此处为了减少输入工作量，提前将M指定*/
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

/*最下生成树算法主函数*/
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
	HANDLE timeThread,primerThread,MSTThread;//定义三个线程
	DWORD id1,id2,id3;//分别表示三个线程的id

	/*以下创建三个线程，且创建后先不进入执行队列*/

	/*获取当前格林尼治时间线程*/

	MSTThread=CreateThread(NULL,0,primMain,NULL,0,&id3);


}
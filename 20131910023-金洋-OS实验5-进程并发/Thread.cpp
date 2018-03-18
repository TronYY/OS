#include <stdio.h>
#include <windows.h>
#include <time.h>//获取时间的头文件


/*得到当前世界时间： 年/月/日 时:分:秒:毫秒 星期*/
/*LPVOID是一个没有类型的指针，也就是说可以将任意类型的指针赋值给LPVOID类型的变量（一般作为参数传递），然后在使用的时候在转换回来。*/
DWORD WINAPI getCurrentTime(LPVOID param)
{
	SYSTEMTIME currentTime;
	for (int ii=0;ii<20;ii++){
		GetSystemTime(&currentTime);
		printf("time%d: %u/%u/%u %u:%u:%u:%u %d\n",ii,currentTime.wYear,currentTime.wMonth,currentTime.wDay,currentTime.wHour,currentTime.wMinute,currentTime.wSecond,currentTime.wMilliseconds,currentTime.wDayOfWeek);
		
	}
	
	return 0;
}



/*得到standN和endN之间的素数*/
DWORD WINAPI getPrimer(LPVOID param)
{
	
	int startN=1000,endN=1100;

	/*为了使运行时间适当增大，此处求素数的算法未做任何优化*/
	for (int koo=startN;koo<=endN;koo++)
	{
		for(int ioo = 2; ioo < koo; ioo++)
			if (koo %ioo == 0) break;
		if (ioo==koo) printf("%d is a primer.\n",koo);
		
	}

	return 0;

}






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
	timeThread=CreateThread(NULL,0,getCurrentTime,NULL,CREATE_SUSPENDED,&id1);

	//getCurrentTime函数名，需要定义其实体；要求第一个函数只打印一句话，调用系统时间	
	//第二个分配大小，0表示动态；第三个参数指向线程的函数指针；第四个传递参数，第五个参数为CREATE_SUSPENDED表示将创建好的进程挂起，若为0则创建好后即可执行；最后一个线程的ID地址、ID变量
	
	/*获取素数线程*/
	primerThread=CreateThread(NULL,0,getPrimer,NULL,CREATE_SUSPENDED,&id2);

	/*最小生成树线程*/
	/*实现用Prim算法判断一个无向加权图是否连通，若连通，则求其一种最小支撑树；*/
	MSTThread=CreateThread(NULL,0,primMain,NULL,CREATE_SUSPENDED,&id3);

	
	printf("\"获取当前格林尼治时间\"线程ID为:%d\n",id1);
	printf("\"获取素数\"线程ID为:%d\n",id2);
	printf("\"最小生成树\"线程ID为:%d\n",id3);

	//system("pause");//主线程运行DOS命令pause
	//printf("你按任意键后，三个线程将被恢复\n");
	ResumeThread(timeThread);//线程恢复函数
	ResumeThread(primerThread);
	ResumeThread(MSTThread);

	system("pause");	
	
	::CloseHandle(timeThread);
	::CloseHandle(primerThread);
	::CloseHandle(MSTThread);
}


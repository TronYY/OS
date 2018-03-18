#include <windows.h>
#include <iostream.h>

//第一个线程函数
DWORD WINAPI ThreadFuncfirst(HANDLE Thread)
{
 int i;
 //循环输出十次"A new thread has created!"
 for(i=0;i<10;i++)
 {
  cout<<"A new thread has created!"<<endl;
  Sleep(60);       //暂停60秒
 }
 return 0;
}

//第二个线程函数
DWORD WINAPI ThreadFunctwo(HANDLE Thread)
{
 int i;
//循环输出十次"B new thread has created!"
 for(i=0;i<10;i++)
 {
  cout<<"B new thread has created!"<<endl;
  Sleep(60);
 }
 return 0;
}

//第三个线程函数,输出前30个素数
DWORD WINAPI ThreadFuncthree(HANDLE Thread)
{
 int i=3,n=0,j,temp;
 while(1)
 {  
	 temp=1;
	 for(j=2;j<i;j++)
	  if(i%j==0)
	  {	 i++;
	     temp=0;
	     break;
	  }

	  if(temp==1) 
	  {
		  n++;cout<<i<<" is "<<n<<"th sushu"<<endl;i++;}
	  if(n==30)
		  break;
  }
 
 return 0;
}

int main(int argc,char* argv[])
{
 HANDLE Thread1,Thread2,Thread3;                                                       //定义三个线程
 DWORD dwThreadId1,dwThreadId2,dwThreadId3;                                            //定义线程的id
 Thread1=::CreateThread(NULL,0,ThreadFuncfirst,NULL,0,&dwThreadId1);                   //创建线程
 Thread2=::CreateThread(NULL,0,ThreadFunctwo,NULL,0,&dwThreadId2);
 Thread3=::CreateThread(NULL,0,ThreadFuncthree,NULL,0,&dwThreadId3);
 cout<<"The first thread ID is :"<<dwThreadId1<<endl;                                  //输出第一个线程的id
 cout<<"The second thread ID is :"<<dwThreadId2<<endl;
 cout<<"The third thread ID is :"<<dwThreadId3<<endl;
 ::WaitForSingleObject(Thread1,INFINITE);
 ::WaitForSingleObject(Thread2,INFINITE);
 ::WaitForSingleObject(Thread3,INFINITE);
 ::CloseHandle(Thread1);
 ::CloseHandle(Thread2);
 ::CloseHandle(Thread3);
 return 0;
}
//注：WaitForSingleObject()是Windows API函数。当等待仍在挂起状态时，句柄被关闭，那么函数行为是未定义的。该句柄必须具有 SYNCHRONIZE 访问权限。
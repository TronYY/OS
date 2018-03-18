#include <windows.h>
#include <iostream.h>
DWORD WINAPI ThreadFuncfirst(HANDLE Thread)
{
	int i;
	for(i=0;i<10;i++)
	{
		cout<<"A new thread has created!"<<endl;
		Sleep(60);
	}
	return 0;
}

DWORD WINAPI ThreadFunctwo(HANDLE Thread)
{
	int i;
	for(i=0;i<10;i++)
	{
		cout<<"B new thread has created!"<<endl;
		Sleep(60);
	}
	return 0;
}


DWORD WINAPI ThreadFuncthree(HANDLE Thread)
{
	int i=3,n=0,j,temp;
	while(1)
	{  
		temp=1;
	 
		for(j=2;j<i;j++)
			if(i%j==0)
			{	 
				i++;
				temp=0;
				break;
			}

		if(temp==1) 
		{
			n++;
			cout<<i<<" is "<<n<<"th sushu"<<endl;
			i++;
		}

	  if(n==30)break;
	}
 
	return 0;
}

int main(int argc,char* argv[])
{
	HANDLE Thread1,Thread2,Thread3;
	DWORD dwThreadId1,dwThreadId2,dwThreadId3;
	Thread1=::CreateThread(NULL,0,ThreadFuncfirst,NULL,0,&dwThreadId1);
	Thread2=::CreateThread(NULL,0,ThreadFunctwo,NULL,0,&dwThreadId2);
	Thread2=::CreateThread(NULL,0,ThreadFuncthree,NULL,0,&dwThreadId3);
	cout<<"The first thread ID is :"<<dwThreadId1<<endl;

	cout<<"The second thread ID is :"<<dwThreadId2<<endl;
	cout<<"The second thread ID is :"<<dwThreadId3<<endl;
	::WaitForSingleObject(Thread1,INFINITE);
	::WaitForSingleObject(Thread2,INFINITE);
	::WaitForSingleObject(Thread3,INFINITE);
	::CloseHandle(Thread1);
	::CloseHandle(Thread2);
	::CloseHandle(Thread3);
	return 0;
}
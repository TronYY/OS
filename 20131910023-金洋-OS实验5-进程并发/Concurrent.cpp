#include <windows.h>
#include <iostream.h>

//��һ���̺߳���
DWORD WINAPI ThreadFuncfirst(HANDLE Thread)
{
 int i;
 //ѭ�����ʮ��"A new thread has created!"
 for(i=0;i<10;i++)
 {
  cout<<"A new thread has created!"<<endl;
  Sleep(60);       //��ͣ60��
 }
 return 0;
}

//�ڶ����̺߳���
DWORD WINAPI ThreadFunctwo(HANDLE Thread)
{
 int i;
//ѭ�����ʮ��"B new thread has created!"
 for(i=0;i<10;i++)
 {
  cout<<"B new thread has created!"<<endl;
  Sleep(60);
 }
 return 0;
}

//�������̺߳���,���ǰ30������
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
 HANDLE Thread1,Thread2,Thread3;                                                       //���������߳�
 DWORD dwThreadId1,dwThreadId2,dwThreadId3;                                            //�����̵߳�id
 Thread1=::CreateThread(NULL,0,ThreadFuncfirst,NULL,0,&dwThreadId1);                   //�����߳�
 Thread2=::CreateThread(NULL,0,ThreadFunctwo,NULL,0,&dwThreadId2);
 Thread3=::CreateThread(NULL,0,ThreadFuncthree,NULL,0,&dwThreadId3);
 cout<<"The first thread ID is :"<<dwThreadId1<<endl;                                  //�����һ���̵߳�id
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
//ע��WaitForSingleObject()��Windows API���������ȴ����ڹ���״̬ʱ��������رգ���ô������Ϊ��δ����ġ��þ��������� SYNCHRONIZE ����Ȩ�ޡ�
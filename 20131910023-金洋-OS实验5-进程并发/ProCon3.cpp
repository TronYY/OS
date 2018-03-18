#include <windows.h>
#include <iostream>
#define THREAD_INSTANCE_NUMBER 3//要创建3个生产者，3个消费者
#define PRODUCT_NUMBER 2
#define MAX_ITEMS 2//仓库只能放2个产品

// 定义信号量句柄
HANDLE m_S_Full;  // Semaphore
HANDLE m_S_Empty; // Semaphore
HANDLE m_E_Mutex;    // Event互斥信号量

int counter=0;

void  ThreadProducer(void  *pData) {            //生产者
	int j;
	int ThreadNumTemp = *(int *)pData;

    for (j=0;j<PRODUCT_NUMBER;j++) {
		WaitForSingleObject(m_S_Empty, INFINITE);//相当于教材的P(empty)
		WaitForSingleObject(m_E_Mutex, INFINITE);//相当于教材的P(mutex)

		counter++;
		printf("生产者线程%d 生产了一个产品\n",ThreadNumTemp);
		printf("生产者线程%d说：当前产品个数为%d\n",ThreadNumTemp,counter);
	   	 
		SetEvent(m_E_Mutex);//相当于教材的V(mutex)
		ReleaseSemaphore(m_S_Full, 1, NULL);//相当于教材的V(full)
	}
}

void  ThreadConsumer(void  *pData) {           //消费者
	int j;
	int ThreadNumTemp=(*(int *) pData);
    for (j=0;j<PRODUCT_NUMBER;j++) {
		WaitForSingleObject(m_S_Full, INFINITE);//相当于教材的P(full)
		WaitForSingleObject(m_E_Mutex, INFINITE);//相当于教材的P(mutex)

		counter--;
		printf("消费者线程%d 消费了一个产品\n",ThreadNumTemp);
		printf("消费者线程%d说：当前产品个数为%d\n",ThreadNumTemp,counter);
		
		SetEvent(m_E_Mutex);//相当于教材的V(mutex)
		ReleaseSemaphore(m_S_Empty, 1, NULL);//相当于教材的V(empty)
	}
}

int main(int argc, char* argv[])
{
	// 假设仓库最多容纳MAX_ITEMS个物品，开始仓库为空
	DWORD IDP[THREAD_INSTANCE_NUMBER];
	DWORD IDC[THREAD_INSTANCE_NUMBER];
	HANDLE hp[THREAD_INSTANCE_NUMBER];
	HANDLE hc[THREAD_INSTANCE_NUMBER];
	int i;
	
	m_S_Full = CreateSemaphore(NULL, 0, MAX_ITEMS, NULL);  //初始计数为0
	m_S_Empty = CreateSemaphore(NULL, MAX_ITEMS, MAX_ITEMS, NULL);  //初始计数为2
	m_E_Mutex = CreateEvent(NULL, FALSE, TRUE, NULL); //自动类型，初始状态为信号态 
	for (i=0;i<THREAD_INSTANCE_NUMBER;i++)
	{
		hp[i]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProducer,(void*)&IDP[i],0,&(IDP[i]));
		hc[i]=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadConsumer,(void*)&IDC[i],0,&(IDC[i]));
		if (hc[i]==NULL) 
			printf("Create thread error!\n");
		if (hp[i]==NULL) 
			printf("Create thread error!\n");
	}

	WaitForMultipleObjects(THREAD_INSTANCE_NUMBER,hp,true,INFINITE);
	WaitForMultipleObjects(THREAD_INSTANCE_NUMBER,hc,true,INFINITE);
	
//The WaitForMultipleObjects function returns when one of the following occurs: 
//Either any one or all of the specified objects are in the signaled state. 
//The time-out interval elapses. 

	CloseHandle(m_S_Full); //The CloseHandle function closes an open object handle. 
	CloseHandle(m_S_Empty);
	//CloseHandle(m_E_Mutex);
	CloseHandle(hp[0]);
	CloseHandle(hp[1]);
	CloseHandle(hc[0]);
	CloseHandle(hc[1]);

	printf("主线程结束!\n");
	return 0;
}
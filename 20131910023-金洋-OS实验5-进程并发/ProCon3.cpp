#include <windows.h>
#include <iostream>
#define THREAD_INSTANCE_NUMBER 3//Ҫ����3�������ߣ�3��������
#define PRODUCT_NUMBER 2
#define MAX_ITEMS 2//�ֿ�ֻ�ܷ�2����Ʒ

// �����ź������
HANDLE m_S_Full;  // Semaphore
HANDLE m_S_Empty; // Semaphore
HANDLE m_E_Mutex;    // Event�����ź���

int counter=0;

void  ThreadProducer(void  *pData) {            //������
	int j;
	int ThreadNumTemp = *(int *)pData;

    for (j=0;j<PRODUCT_NUMBER;j++) {
		WaitForSingleObject(m_S_Empty, INFINITE);//�൱�ڽ̲ĵ�P(empty)
		WaitForSingleObject(m_E_Mutex, INFINITE);//�൱�ڽ̲ĵ�P(mutex)

		counter++;
		printf("�������߳�%d ������һ����Ʒ\n",ThreadNumTemp);
		printf("�������߳�%d˵����ǰ��Ʒ����Ϊ%d\n",ThreadNumTemp,counter);
	   	 
		SetEvent(m_E_Mutex);//�൱�ڽ̲ĵ�V(mutex)
		ReleaseSemaphore(m_S_Full, 1, NULL);//�൱�ڽ̲ĵ�V(full)
	}
}

void  ThreadConsumer(void  *pData) {           //������
	int j;
	int ThreadNumTemp=(*(int *) pData);
    for (j=0;j<PRODUCT_NUMBER;j++) {
		WaitForSingleObject(m_S_Full, INFINITE);//�൱�ڽ̲ĵ�P(full)
		WaitForSingleObject(m_E_Mutex, INFINITE);//�൱�ڽ̲ĵ�P(mutex)

		counter--;
		printf("�������߳�%d ������һ����Ʒ\n",ThreadNumTemp);
		printf("�������߳�%d˵����ǰ��Ʒ����Ϊ%d\n",ThreadNumTemp,counter);
		
		SetEvent(m_E_Mutex);//�൱�ڽ̲ĵ�V(mutex)
		ReleaseSemaphore(m_S_Empty, 1, NULL);//�൱�ڽ̲ĵ�V(empty)
	}
}

int main(int argc, char* argv[])
{
	// ����ֿ��������MAX_ITEMS����Ʒ����ʼ�ֿ�Ϊ��
	DWORD IDP[THREAD_INSTANCE_NUMBER];
	DWORD IDC[THREAD_INSTANCE_NUMBER];
	HANDLE hp[THREAD_INSTANCE_NUMBER];
	HANDLE hc[THREAD_INSTANCE_NUMBER];
	int i;
	
	m_S_Full = CreateSemaphore(NULL, 0, MAX_ITEMS, NULL);  //��ʼ����Ϊ0
	m_S_Empty = CreateSemaphore(NULL, MAX_ITEMS, MAX_ITEMS, NULL);  //��ʼ����Ϊ2
	m_E_Mutex = CreateEvent(NULL, FALSE, TRUE, NULL); //�Զ����ͣ���ʼ״̬Ϊ�ź�̬ 
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

	printf("���߳̽���!\n");
	return 0;
}
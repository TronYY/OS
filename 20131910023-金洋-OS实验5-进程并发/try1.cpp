#include <windows.h>
#include <iostream>

DWORD WINAPI fun1(LPVOID param){
	while(1){
		Sleep(1000);                 //�ȴ��ж��ź�
		printf("This is fun1 qq\n");	
	}
	return 1;
}
DWORD WINAPI Fun2(LPVOID param){
	while(1){
		Sleep(1000);
		printf("This is fun2 hhhhhhhhhhhhhhh\n");	
	}
	return 1;
}
int main(){
	DWORD pid1 = 0, pid2 = 0;
	HANDLE h1 = CreateThread(NULL,0,fun1,NULL, CREATE_SUSPENDED, &pid1);//�����߳�
	HANDLE h2 = CreateThread(NULL,0,Fun2,NULL,CREATE_SUSPENDED, &pid2);
	printf("���̵߳�id����%d,�����̵߳�id�ŷֱ���%d,%d\n",GetCurrentThreadId(),pid1,pid2);
	system("pause");//���߳�����DOS����pause
	printf("�㰴������������߳̽����ָ�\n");
	ResumeThread(h1);//�ָ̻߳�����
	ResumeThread(h2);
	system("pause");
	printf("�㰴��������߳�h1��������\n");
	SuspendThread(h1);
	system("pause");
	printf("�㰴��������߳�h1�����ָ����߳�h2������ֹ\n");
	ResumeThread(h1);
	TerminateThread(h2,1);
	system("pause");
	CloseHandle(h1);
	CloseHandle(h2);
	return 0;	
}
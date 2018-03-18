#include <windows.h>
#include <iostream>

DWORD WINAPI fun1(LPVOID param){
	while(1){
		Sleep(1000);                 //等待中断信号
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
	HANDLE h1 = CreateThread(NULL,0,fun1,NULL, CREATE_SUSPENDED, &pid1);//创建线程
	HANDLE h2 = CreateThread(NULL,0,Fun2,NULL,CREATE_SUSPENDED, &pid2);
	printf("主线程的id号是%d,两个线程的id号分别是%d,%d\n",GetCurrentThreadId(),pid1,pid2);
	system("pause");//主线程运行DOS命令pause
	printf("你按任意键后，两个线程将被恢复\n");
	ResumeThread(h1);//线程恢复函数
	ResumeThread(h2);
	system("pause");
	printf("你按任意键后，线程h1将被挂起\n");
	SuspendThread(h1);
	system("pause");
	printf("你按任意键后，线程h1将被恢复，线程h2将被终止\n");
	ResumeThread(h1);
	TerminateThread(h2,1);
	system("pause");
	CloseHandle(h1);
	CloseHandle(h2);
	return 0;	
}
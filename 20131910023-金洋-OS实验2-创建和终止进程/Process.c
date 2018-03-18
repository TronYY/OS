	
#include <stdio.h>
#include <signal.h>//头文件<signal.h>中提供了一些用于处理程序运行期间所引发的异常条件的功能，如处理来源于外部的中断信号或程序执行期间出现的错误等事件
#include <unistd.h>
#include <sys/types.h>
int wait_flag;//全局变量wait_flag,用来标识进程状态

void stop(){
	wait_flag=0;
}

void main(){
	int pid1,pid2;//定义两个进程号参数
	signal(3,stop); //或 signal(14,stop);signal()函数允许调用进程控制软中断信号的处理

	while ( (pid1=fork())==-1);//程序等待 成功创建子进程的事件发生
	
	if (pid1>0){
	// 子进程创建成功,pid1为进程号
	
		while  ((pid2=fork())==-1);	// 创建子进程2  
		if (pid2>0){
			wait_flag=1;//标记为阻塞状态
			sleep(5);// 父进程等待5秒
			kill(pid1,16);//杀死进程1 
			kill(pid2,17);//杀死进程2
			wait(0);//等待子进程1结束的信号
			wait(0);//等待子进程2结束的信号
			printf("\n Parent process is killed!!\n");
			exit(0); // 父进程结束
		}
		else{
			wait_flag=1;
			signal(17,stop);//等待进程2被杀死的中断号17
			printf("\n Child process 2 is killed by parent!!\n");
			exit(0);
		}
	
	else{
		wait_flag=1;
		signal(16,stop);//等待进程1被杀死的中断号16
		printf("\n Child process 1 is killed by parent!!\n");
		exit(0);
	}

}
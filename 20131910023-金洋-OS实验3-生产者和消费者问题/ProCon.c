#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <iostream.h>

int dmutex=1;//互斥信号量
int buffer[3]={0,0,0};//缓冲区状态，1表示有消息，0表示无消息



/*打印缓冲区状态*/
void display()
{
	printf("buffers statu:%d,%d,%d;\n",buffer[0],buffer[1],buffer[2]);
}


/*生产者进程*/
DWORD WINAPI producer()
{
	int i;
	while (1)
	{

		/*若没有生产者正在向缓冲区存放消息，且没有消费者正向缓冲区提取消息，则此时生产者可以向缓冲区存放消息*/
		if (dmutex==1)
		{
			dmutex=0;//标志信号量
			

			/*查找空的缓冲格，存放消息*/
			while (1)
			{
				
				for (i=0;i<3;i++)
					if (buffer[i]==0){
						buffer[i]++;
						printf("Produce an item to buffer %d,",i);
						display();
						break;
					}
				if (i<3)
				{
					/*存放完后，将信号量标志为1,便于其他进程访问缓冲区*/
					dmutex=1;
					break;
				}
				/*未找到空的缓冲格，重新查找*/
				else Sleep(rand()%1000+1);
			}

		}
	
		/*否则继续等待*/
		else Sleep(rand()%1000+1);
	}
}


			
/*消费者进程*/
DWORD WINAPI consumer()
{
	int i;

	
	while (1)
	{
		/*若没有生产者正在向缓冲区存放消息，且没有消费者正向缓冲区提取消息，则此时消费者可以从缓冲区提取消息*/
		if (dmutex==1)
		{
			dmutex=0;

			/*查找非空的缓冲格，提取消息*/
			while (1)
			{
				for (i=0;i<3;i++)
					if (buffer[i]==1){
						buffer[i]--;
						printf("Consume an item from buffer %d,",i);
						display();
						break;
					}

				if (i<3)
				{
					/*存放完后，将信号量标志为1,便于其他进程访问缓冲区*/
					dmutex=1;
					break;
				}
				/*未找到非空的缓冲格，重新查找*/
				else Sleep(rand()%1000+1);
				
			}
		}
		

		/*否则继续等待*/
		else Sleep(rand()%1000+1);

	}

}


int main(){
	
	HANDLE handle[5];
	DWORD dw1,dw2,dw3,dw4,dw5;
	srand((unsigned) time(NULL));
	
	handle[0]=CreateThread(NULL,0,producer,NULL,0,&dw1);
    producer();

	Sleep(rand()%1000+1);
	handle[1]=CreateThread(NULL,0,consumer,NULL,0,&dw4);
	consumer();

	Sleep(rand()%1000+1);
	handle[2]=CreateThread(NULL,0,producer,NULL,0,&dw2);
	producer();

	Sleep(rand()%1000+1);
	handle[3]=CreateThread(NULL,0,consumer,NULL,0,&dw5);
	producer();

	Sleep(rand()%1000+1);
	handle[4]=CreateThread(NULL,0,producer,NULL,0,&dw3);
	consumer();

	
	
	
}

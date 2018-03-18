#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <iostream.h>

int dmutex=1;//�����ź���
int buffer[3]={0,0,0};//������״̬��1��ʾ����Ϣ��0��ʾ����Ϣ



/*��ӡ������״̬*/
void display()
{
	printf("buffers statu:%d,%d,%d;\n",buffer[0],buffer[1],buffer[2]);
}


/*�����߽���*/
DWORD WINAPI producer()
{
	int i;
	while (1)
	{

		/*��û�������������򻺳��������Ϣ����û�����������򻺳�����ȡ��Ϣ�����ʱ�����߿����򻺳��������Ϣ*/
		if (dmutex==1)
		{
			dmutex=0;//��־�ź���
			

			/*���ҿյĻ���񣬴����Ϣ*/
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
					/*�����󣬽��ź�����־Ϊ1,�����������̷��ʻ�����*/
					dmutex=1;
					break;
				}
				/*δ�ҵ��յĻ�������²���*/
				else Sleep(rand()%1000+1);
			}

		}
	
		/*��������ȴ�*/
		else Sleep(rand()%1000+1);
	}
}


			
/*�����߽���*/
DWORD WINAPI consumer()
{
	int i;

	
	while (1)
	{
		/*��û�������������򻺳��������Ϣ����û�����������򻺳�����ȡ��Ϣ�����ʱ�����߿��Դӻ�������ȡ��Ϣ*/
		if (dmutex==1)
		{
			dmutex=0;

			/*���ҷǿյĻ������ȡ��Ϣ*/
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
					/*�����󣬽��ź�����־Ϊ1,�����������̷��ʻ�����*/
					dmutex=1;
					break;
				}
				/*δ�ҵ��ǿյĻ�������²���*/
				else Sleep(rand()%1000+1);
				
			}
		}
		

		/*��������ȴ�*/
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

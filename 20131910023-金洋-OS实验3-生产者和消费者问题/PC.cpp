#include "iostream.h" 
#include "windows.h" 
#include "stdio.h" 

boolean lock[4]={false,false,false};  //���岢�ҳ�ʼ��3���ջ�����
int buffer[3]={0,0,0};                 //������ǳ�ʼ��ʾδ����

//��ʾ��ʱʹ�õ��ǵڼ���������
void display()
{
	printf("buffers statu:%d,%d,%d;\n",buffer[0],buffer[1],buffer[2]);
}


//�����������̺߳���
DWORD WINAPI Producer(LPVOID lpParameter)
{
	while(true)                                    //������
	{ 
		for(int j=0;j<3;j++)                      //ɨ�����еĻ�����
		{
			if(buffer[j]==0)                     //�ж��Ƿ��пյĻ�����
			{
				if(lock[j]==false)               //�жϴ˿ջ������Ƿ�ʹ��
				{
					lock[j]=true;                //��ǿ�ʼʹ�û�����
					if(buffer[j]<1)
					{
						++buffer[j];
						cout<<"����һ����Դ�����뻺����"<<j<<"��"<<endl; 
						lock[j]=false; 
                        break;
					}
				} 
				if(j==3)
				{
					cout<<"�Ҳ����ջ��������ȴ��С���"<<endl; 
					Sleep(2000); 
				}
			}
		} 
 } 
 return 0;
}



//�����������̺߳���
 DWORD WINAPI Customer(LPVOID lpParameter)
 {
	 while(true)                                    //������
	 {
		 for(int n=0;n<3;n++)                       //ɨ�����л�����
		 {
			 if(buffer[n]==1)                       //�жϻ������Ƿ���ֵ
			 {
				 if(lock[n]==false)                 //�жϴ˻������Ƿ���� 
				 {
					 lock[n]=true;                 //��ǿ�ʼʹ�û�����
					 if(buffer[n]>=1)
					 {
						 --buffer[n]; 
                         cout<<"����һ����Դ���ӻ�����"<<n<<"��ȡ��"<<endl; 
                         lock[n]=false; 
                         break;
					 }
				 }
			 }
			 if(n==3)
			 {
				 cout<<"�Ҳ��������������ȴ��С���"<<endl;  
                 Sleep(2000);
			 }
		 }
	 }
 } 
 
//������������5�����̣�����3�������߽��̣�2�������߽���
int main(int argc,char* argv[]) 
{ 
	display();
	HANDLE handle[3]; 
	DWORD dw1,dw2,dw3,dw4,dw5;//����5������ 
	handle[0]=CreateThread(NULL,0,Producer,NULL,0,&dw1); 
	handle[1]=CreateThread(NULL,0,Producer,NULL,0,&dw2); 
	handle[2]=CreateThread(NULL,0,Producer,NULL,0,&dw3); 
	handle[3]=CreateThread(NULL,0,Customer,NULL,0,&dw4); 
	handle[4]=CreateThread(NULL,0,Customer,NULL,0,&dw5);
	display(); 
	Sleep(3000); 
	display(); 
	return 0; 
}
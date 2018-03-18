#include "iostream.h" 
#include "windows.h" 
#include "stdio.h" 

boolean lock[4]={false,false,false};  //定义并且初始化3个空缓冲区
int buffer[3]={0,0,0};                 //上锁标记初始表示未加锁

//显示此时使用的是第几个缓冲区
void display()
{
	printf("buffers statu:%d,%d,%d;\n",buffer[0],buffer[1],buffer[2]);
}


//生产者生产线程函数
DWORD WINAPI Producer(LPVOID lpParameter)
{
	while(true)                                    //不跳出
	{ 
		for(int j=0;j<3;j++)                      //扫描所有的缓冲区
		{
			if(buffer[j]==0)                     //判断是否有空的缓冲区
			{
				if(lock[j]==false)               //判断此空缓冲区是否被使用
				{
					lock[j]=true;                //标记开始使用缓冲区
					if(buffer[j]<1)
					{
						++buffer[j];
						cout<<"生产一个资源，放入缓冲区"<<j<<"中"<<endl; 
						lock[j]=false; 
                        break;
					}
				} 
				if(j==3)
				{
					cout<<"找不到空缓冲区，等待中。。"<<endl; 
					Sleep(2000); 
				}
			}
		} 
 } 
 return 0;
}



//消费者消费线程函数
 DWORD WINAPI Customer(LPVOID lpParameter)
 {
	 while(true)                                    //不跳出
	 {
		 for(int n=0;n<3;n++)                       //扫描所有缓冲区
		 {
			 if(buffer[n]==1)                       //判断缓冲区是否有值
			 {
				 if(lock[n]==false)                 //判断此缓冲区是否空闲 
				 {
					 lock[n]=true;                 //标记开始使用缓冲区
					 if(buffer[n]>=1)
					 {
						 --buffer[n]; 
                         cout<<"消费一个资源，从缓冲区"<<n<<"中取出"<<endl; 
                         lock[n]=false; 
                         break;
					 }
				 }
			 }
			 if(n==3)
			 {
				 cout<<"找不到满缓冲区，等待中。。"<<endl;  
                 Sleep(2000);
			 }
		 }
	 }
 } 
 
//主函数，创建5个进程，其中3个生产者进程，2个消费者进程
int main(int argc,char* argv[]) 
{ 
	display();
	HANDLE handle[3]; 
	DWORD dw1,dw2,dw3,dw4,dw5;//创建5个进程 
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
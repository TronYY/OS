#include <iostream>
#include <afxwin.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object
CWinApp theApp;
using namespace std;

const unsigned short SIZE_OF_BUFFER = 10; //缓冲区长度

unsigned short ProductID = 0;    //这里的产品号和将被消耗的产品号是
unsigned short ConsumeID = 0;   //为了跟踪产品的存取过程而引入的辅助变量

unsigned short in = 0;   //缓冲区下标，指向生产者要放产品的缓冲区单元
unsigned short out = 0;  //缓冲区下标，指向消费者要取产品的缓冲区单元
int g_buffer[SIZE_OF_BUFFER];       //缓冲区是个循环队列
bool g_continue = true;        //总控开关，可随时结束诸进程

HANDLE g_hMutex;          //互斥信号量句柄，用于诸线程互斥访问缓冲区
HANDLE g_hFullSemaphore;     //资源信号量句柄，代表缓冲区内已放置的产品数
HANDLE g_hEmptySemaphore;  //资源信号量句柄，代表缓冲区内空闲的单元数

DWORD WINAPI Producer(LPVOID);       //生产者线程声明
DWORD WINAPI Consumer(LPVOID);       //消费者线程声明
int _tmain(int argc,  TCHAR* argv[],TCHAR* envp[])   
{      int nRetCode=0;
       g_hMutex = CreateMutex(NULL,FALSE,NULL); //互斥信号量
       g_hFullSemaphore = CreateSemaphore(
NULL,
0, //初始缓冲区内无产品
SIZE_OF_BUFFER-1,NULL);
       g_hEmptySemaphore =CreateSemaphore(
NULL,
SIZE_OF_BUFFER-1,  // 初始均是空缓冲区
SIZE_OF_BUFFER-1,NULL);
      
//调整下面的数值，可以发现，当生产者个数多于消费者个数时，
//生产速度快，生产者经常等待消费者；反之，消费者经常等待 
       const unsigned short PRODUCERS_COUNT = 3;     //生产者的个数
       const unsigned short CONSUMERS_COUNT = 1;     //消费者的个数
       const unsigned short THREADS_COUNT = PRODUCERS_COUNT+CONSUMERS_COUNT; //总的线程数
       
HANDLE  hThreads[THREADS_COUNT]; //诸线程的handle表
       DWORD  producerID[CONSUMERS_COUNT]; //生产者线程的标识符表
       DWORD  consumerID[PRODUCERS_COUNT]; //消费者线程的标识符表
      
//创建生产者诸线程
       for (int i=0;i<PRODUCERS_COUNT;++i)
{
           hThreads[i]=CreateThread(NULL,0,Producer,NULL,0,&producerID[i]);
           if (hThreads[i]==NULL) return -1;
       }
       //创建消费者线程
       for (i=0;i<CONSUMERS_COUNT;++i)
{
          hThreads[PRODUCERS_COUNT+i]=CreateThread(NULL,
0,
Consumer,
NULL,
0,&consumerID[i]);
           if (hThreads[i]==NULL) return -1;
       }
       

while(g_continue)
{
if(getchar())      //按回车后终止程序运行
{	
g_continue = false;
}
}
CloseHandle(g_hMutex);
CloseHandle(g_hEmptySemaphore);
CloseHandle(g_hFullSemaphore);
for(i=0;i<THREADS_COUNT;i++)
		CloseHandle(hThreads[i]);
return nRetCode;
}//主线程main结束


//生产一个产品。简单模拟了一下，仅输出新产品的ID号
void Produce()
{
      cerr << "Producing " << ++ProductID << " ... ";
      cerr << "Succeed" <<endl;
}
//把新生产的产品放入缓冲区
void Append()
{
      cerr << "Appending a product ... ";
       g_buffer[in] = ProductID;
       in = (in+1)%SIZE_OF_BUFFER;
       cerr << "Succeed" <<endl;
       
//扫描缓冲区，输出缓冲区的当前状态
       for (int i=0;i<SIZE_OF_BUFFER;++i)
{
            cout << i <<": " << g_buffer[i];
            if (i==in) cout << " <-- 生产";
            if (i==out) cout << " <-- 消费";
            cout <<endl;
        }
}


//从缓冲区中取出一个产品
void Take()
{
       cerr << "Taking a product ... ";
       ConsumeID = g_buffer[out];
g_buffer[out] = -1;//取走产品后，设置为-1以示区别
       out = (out+1)%SIZE_OF_BUFFER;
       cerr << "Succeed" << endl;
       
//扫描缓冲区，输出缓冲区的当前状态
       for (int i=0;i<SIZE_OF_BUFFER;++i)
{
           cout << i <<": " << g_buffer[i];
           if (i==in) cout << " <-- 生产";
           if (i==out) cout << " <-- 消费";
           cout << endl;
       }
}
//消耗一个产品
void Consume()
{
       cerr << "Consuming " << ConsumeID << " ... ";
       cerr << "Succeed" <<endl;
}

//生产者线程的实现
DWORD     WINAPI Producer(LPVOID lpPara)
{
       while(g_continue)
{
           WaitForSingleObject(g_hEmptySemaphore,INFINITE);
           WaitForSingleObject(g_hMutex,INFINITE);
           Produce();
           Append();
           Sleep(1500);
           ReleaseMutex(g_hMutex);
           ReleaseSemaphore(g_hFullSemaphore,1,NULL);
       }
       return 0;
}
//消费者进程的实现
DWORD     WINAPI  Consumer(LPVOID lpPara)
{
        while(g_continue)
{
           WaitForSingleObject(g_hFullSemaphore,INFINITE);
           WaitForSingleObject(g_hMutex,INFINITE);
           Take();
           Consume();
           Sleep(1500);
           ReleaseMutex(g_hMutex);
           ReleaseSemaphore(g_hEmptySemaphore,1,NULL);
       }
       return 0;
}
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

const unsigned short SIZE_OF_BUFFER = 10; //����������

unsigned short ProductID = 0;    //����Ĳ�Ʒ�źͽ������ĵĲ�Ʒ����
unsigned short ConsumeID = 0;   //Ϊ�˸��ٲ�Ʒ�Ĵ�ȡ���̶�����ĸ�������

unsigned short in = 0;   //�������±ָ꣬��������Ҫ�Ų�Ʒ�Ļ�������Ԫ
unsigned short out = 0;  //�������±ָ꣬��������Ҫȡ��Ʒ�Ļ�������Ԫ
int g_buffer[SIZE_OF_BUFFER];       //�������Ǹ�ѭ������
bool g_continue = true;        //�ܿؿ��أ�����ʱ���������

HANDLE g_hMutex;          //�����ź���������������̻߳�����ʻ�����
HANDLE g_hFullSemaphore;     //��Դ�ź�������������������ѷ��õĲ�Ʒ��
HANDLE g_hEmptySemaphore;  //��Դ�ź�����������������ڿ��еĵ�Ԫ��

DWORD WINAPI Producer(LPVOID);       //�������߳�����
DWORD WINAPI Consumer(LPVOID);       //�������߳�����
int _tmain(int argc,  TCHAR* argv[],TCHAR* envp[])   
{      int nRetCode=0;
       g_hMutex = CreateMutex(NULL,FALSE,NULL); //�����ź���
       g_hFullSemaphore = CreateSemaphore(
NULL,
0, //��ʼ���������޲�Ʒ
SIZE_OF_BUFFER-1,NULL);
       g_hEmptySemaphore =CreateSemaphore(
NULL,
SIZE_OF_BUFFER-1,  // ��ʼ���ǿջ�����
SIZE_OF_BUFFER-1,NULL);
      
//�����������ֵ�����Է��֣��������߸������������߸���ʱ��
//�����ٶȿ죬�����߾����ȴ������ߣ���֮�������߾����ȴ� 
       const unsigned short PRODUCERS_COUNT = 3;     //�����ߵĸ���
       const unsigned short CONSUMERS_COUNT = 1;     //�����ߵĸ���
       const unsigned short THREADS_COUNT = PRODUCERS_COUNT+CONSUMERS_COUNT; //�ܵ��߳���
       
HANDLE  hThreads[THREADS_COUNT]; //���̵߳�handle��
       DWORD  producerID[CONSUMERS_COUNT]; //�������̵߳ı�ʶ����
       DWORD  consumerID[PRODUCERS_COUNT]; //�������̵߳ı�ʶ����
      
//�������������߳�
       for (int i=0;i<PRODUCERS_COUNT;++i)
{
           hThreads[i]=CreateThread(NULL,0,Producer,NULL,0,&producerID[i]);
           if (hThreads[i]==NULL) return -1;
       }
       //�����������߳�
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
if(getchar())      //���س�����ֹ��������
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
}//���߳�main����


//����һ����Ʒ����ģ����һ�£�������²�Ʒ��ID��
void Produce()
{
      cerr << "Producing " << ++ProductID << " ... ";
      cerr << "Succeed" <<endl;
}
//���������Ĳ�Ʒ���뻺����
void Append()
{
      cerr << "Appending a product ... ";
       g_buffer[in] = ProductID;
       in = (in+1)%SIZE_OF_BUFFER;
       cerr << "Succeed" <<endl;
       
//ɨ�軺����������������ĵ�ǰ״̬
       for (int i=0;i<SIZE_OF_BUFFER;++i)
{
            cout << i <<": " << g_buffer[i];
            if (i==in) cout << " <-- ����";
            if (i==out) cout << " <-- ����";
            cout <<endl;
        }
}


//�ӻ�������ȡ��һ����Ʒ
void Take()
{
       cerr << "Taking a product ... ";
       ConsumeID = g_buffer[out];
g_buffer[out] = -1;//ȡ�߲�Ʒ������Ϊ-1��ʾ����
       out = (out+1)%SIZE_OF_BUFFER;
       cerr << "Succeed" << endl;
       
//ɨ�軺����������������ĵ�ǰ״̬
       for (int i=0;i<SIZE_OF_BUFFER;++i)
{
           cout << i <<": " << g_buffer[i];
           if (i==in) cout << " <-- ����";
           if (i==out) cout << " <-- ����";
           cout << endl;
       }
}
//����һ����Ʒ
void Consume()
{
       cerr << "Consuming " << ConsumeID << " ... ";
       cerr << "Succeed" <<endl;
}

//�������̵߳�ʵ��
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
//�����߽��̵�ʵ��
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
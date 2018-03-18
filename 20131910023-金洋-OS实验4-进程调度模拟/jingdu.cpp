#include <iostream.h> 
#include <queue> 
#include <string> 
#include <windows.h>  
#include <malloc.h> 
#include<algorithm>   
#define UNREADY 0        //未就绪
#define READY 1          //就绪
#define EXECUTE 2        //执行
#define END 3            //完成
using namespace std;  //进程的状态
//进程的PCB结构类型定义和结构变量说明
 struct ProcessPcb 
{  char pro_name[20];    //进程的名字
   int  time_submit;     //提交时间,从时间的1开始计时
   int  time_exe;       //进程所需的运行时间
   int  pro_id;        //进程ID(系统生成) 
   int  pro_priority;    //进程优先级 
   int  time_start;     //开始执行的时间
   int  time_end;        //结束的时间
   int  time_wait;       //等待的时间
   int  pro_state;       //进程的状态 (就绪，执行，完成)   
   int  time_left;      //还需多少时间单位,初始化为所需的执行时间  
   int  time_turn;    //周转时间
   double time_aver;     //带权周转时间 
   ProcessPcb *next; 
   ProcessPcb() 
      { 
       next=NULL;
      }
}ProcessPcb;  
//封装CPU调度与分配程序模块。进程类
  class CpuModel 
{  public: 
  CpuModel() 
  {  
     pcbnum=0; 
  } 
  void cpurun();         //cpu模拟运行函数
  bool CreatePcb();         //进程输入函数
  void ShowPcb();        //将输入的进程展示出来
  void PriModel();       //可强占的优先进程调度模式
  void FcfsModel();      //先到先服务调度模式
  private: 
   struct ProcessPcb PcbList[100];     //按提交时间排的未就绪进程队列
   int pcbnum;                //进程数量
   int  freetime;             //cpu空闲时间
   int allturn;                //总周转时间
   float allaver;              //总带权周转时间
 }; 
     //cpu模拟运行函数
 void CpuModel::cpurun() 
{    int choose=0;
    while(1) 
	{
     cout<<"***主菜单***"<<endl; 
     cout<<"----------------------------------"<<endl; 
     cout<<"***1:创建进程***"<<endl; 
     cout<<"***2:显示已创建的进程***"<<endl; 
     cout<<"***3:可强占的优先进程调度***"<<endl; 
     cout<<"**4:先到先服务调度***"<<endl;  
     cout<<"***5:退出系统***"<<endl; 
     cout<<"\n***请选择: ";
     cin>>choose;  
     cout<<endl; 
     switch(choose)
	 { case 1: CreatePcb();     //创建进程
           break; 
       case 2: ShowPcb();     //显示已创建的进程
           break; 
       case 3: PriModel();    //可强占的优先进程方式
           break;
       case 4: FcfsModel();     //先到先服务调度方式 
           break; 
       case 5: return;          //结束程序
	 } 
      cout<<endl; 
	} 
};  
 //按进程提交时间排序的比较函数 
bool cmp(struct ProcessPcb a,struct ProcessPcb b) 
{  
     if(a.time_submit<b.time_submit) 
       return true; 
     else return false; 
};  //判断是否数字
int isnumber(char a[]) 
{  
        int len=strlen(a); 
        int i,num=0;  
        if(a[0]>'0'&&a[0]<='9') 
        num=a[0]-'0'; 
        else return -1; 
        for(i=1;i<len;i++) 
        { if(a[i]>='0'&&a[i]<='9') 
            num=num*10+a[i]-'0'; 
          else return -1; 
        }  
        return num; 
};  
//进程输入函数
 bool CpuModel::CreatePcb() 
{  char num[10]; 
   cout<<"\n***请输入你想建立的进程个数: "; 
    cin>>num;    //输入进程个数
 pcbnum=isnumber(num);
 if(pcbnum<=0)  
    { cout<<"***输入有错***\n"; 
 return 0; 
   }  
  int i;  
  for(i=0;i<pcbnum;i++) //输入进程信息
 
     { cout<<"***请依次输入第"<<i+1<<"个进程的信息***"<<endl; 
       cout<<"*******名字：";
       cin>>PcbList[i].pro_name;
       cout<<"***提交时间：";
       cin>>num; 
PcbList[i].time_submit=isnumber(num); 
if(PcbList[i].time_submit<=0) 
             {  
cout<<"***输入有错***\n"; 
return 0; 
              } 
          cout<<"***执行时间："; 
            cin>>num;  
            PcbList[i].time_exe=isnumber(num); 
            if(PcbList[i].time_exe<=0) 
{ cout<<"***输入有错***\n";
                            return 0; 
} 
 PcbList[i].time_left=PcbList[i].time_exe;  //设置剩余执行时间  
 PcbList[i].pro_state=UNREADY;        //设置状态为未就绪  
PcbList[i].pro_priority=100-PcbList[i].time_exe; //设置优先级，优先级=100-执行时间
 cout<<endl;
          } 
 sort(PcbList,PcbList+pcbnum,cmp);        //按提交时间排序
 
           for(i=0;i<pcbnum;i++)  
            PcbList[i].pro_id=i+1;  //设置Id,根据提交时间  
           return 1; 
};  
//展示所需执行进程信息 
void CpuModel::ShowPcb() 
{           int i;  
         cout<<"\n***所需执行的进程信息如下***\n"; 
         for(i=0;i<pcbnum;i++)                
cout<<"进程编号:"<<PcbList[i].pro_id<<"进程名字: "<<PcbList[i].pro_name<<"提交时间:"<<PcbList[i].time_submit<<"执行时间: "<<PcbList[i].time_exe<<"优先级数："<<PcbList[i].pro_priority<<endl;
         cout<<endl; 
};  
//可强占的优先进程调度模式
void CpuModel::PriModel() 
{ cout<<"********可强占的优先进程调度过程如下********\n"; 
    freetime=0;   //初始化系统空闲时间为0 
    int time=0;  //时间
     int nextid=0;  
    int pnum=0;   //已就绪进程数目
    struct ProcessPcb *head;  //就绪队列 
    struct ProcessPcb *pcb,*pcb1,*pcb2; 
    head=(struct ProcessPcb*)malloc(sizeof(struct ProcessPcb));  
    head->next=NULL;       //head指针不用，head->next开始为就绪队列
//模拟cpu开始工作
while(head->next!=NULL||pnum<pcbnum) 
   {  
     Sleep(1000);  
     time++;     //时间片为1  
     cout<<"*Time:第"<<time<<"秒"<<endl;     //打印 
// 进程就绪（查看未就绪的进程中在该时间片是否可以进入就绪队列）
    while(pnum<pcbnum && PcbList[pnum].time_submit<=time)  
	{  
     pcb=(struct ProcessPcb*)malloc(sizeof(struct ProcessPcb)); 
     *pcb=PcbList[pnum++]; //进入就绪队列
     pcb->next=head->next;        //插在head之后 
     head->next=pcb; 
	}  //查找在time时间片应该运行的进程 
      if(head->next==NULL) 
         {  cout<<"没有可运行的进程"<<endl; 
            freetime++; 
          } 
        else 
         {  
                    pcb2=head;  
           pcb=head->next; 
          pcb1=head->next; //查找当前优先级最高的就绪进程
     while(pcb1->next!=NULL) 
         {  
           if(pcb->pro_priority<=pcb1->next->pro_priority) 
               {  pcb2=pcb1;  
                 pcb=pcb1->next; 
               }  
           pcb1=pcb1->next; 
         } //运行优先级最高的进程
    if(pcb->time_left==pcb->time_exe)    //该进程是否已开始
	{  pcb->time_start=time;           //设置开始时间
     pcb->pro_state=EXECUTE;         //设置状态为执行
	}  
    pcb->pro_priority=pcb->pro_priority-3;   //优先级数减3 
    pcb->time_left--;         //剩余执行时间减去时间片1 
   //打印该进程信息
    cout<<"进程编号: "<<pcb->pro_id<<"  进程名字:" <<pcb->pro_name<<"  提交时间: "<<pcb->time_submit<<"  执行时间: "<<pcb->time_exe<<"\n开始时间: "<<pcb->time_start<<"  还剩时间: "<<pcb->time_left<<"优先级数："<<pcb->pro_priority<<endl;  //该进程是否执行结束
   if(pcb->time_left==0)          
   {  pcb2->next=pcb2->next->next;//将其在就绪队列中出列
        pcb->time_end=time;     //设置结束时间
        pcb->time_turn=pcb->time_end-pcb->time_submit+1; //计算周转时间 
        pcb->time_aver=pcb->time_turn/(double)pcb->time_exe; //计算带权周转时间
          pcb->pro_state=END;  
        cout<<"\n进程"<<pcb->pro_id<<"  运行结束："<<"\n  进程名字: "<<pcb->pro_name<<"  提交时间: "<<pcb->time_submit<<"  执行时间: "<<pcb->time_exe<<"  开始时间: "<<pcb->time_start<<"结束时间: "<<pcb->time_end<<"\n周转时间: "<<pcb->time_turn<<"带权周转时间: "<<pcb->time_aver<<endl;  
   }
		} 
}   
                                  
//模拟CPU工作结束
  cout<<"\n 所有进程运行完毕！"<<endl;
        double c=(time-freetime)/(double)time*100; 
      cout<<"\n cpu利用率："<<c<<"%"<<endl;  
};                              
//可强占的优先进程调度模式结束 
//先到先服务调度模式 
void CpuModel::FcfsModel() 
{  
   cout<<"********先到先服务进程调度过程如下********\n"; 
     freetime=0;             //初始化系统空闲时间为0 
      int   time=0;           //时间  
     int   pnum=0;           //已就绪进程数目
 
     struct ProcessPcb que[100];    //就绪队列
       int   quef=0,quee=0;     //就绪队列的头指针和尾指针
  //模拟cpu开始工作
 while(quef<quee||pnum<pcbnum) 
     {  
           Sleep(1000);  
           time++;//时间片为1  
           cout<<"*Time: 第"<<time<<"秒"<<endl;    //打印
    //进程就绪（查看未就绪的进程中在该时间片是否可以进入就绪队列）
    while(pnum<pcbnum && PcbList[pnum].time_submit<=time)  
que[quee++]=PcbList[pnum++]; 
    //查找time时间片的运行进程
 
           if(quef==quee) 
           {  cout<<"没有可运行的进程"<<endl; 
                freetime++; 
           } 
           else  
           {                 
    //运行进程
            if(que[quef].time_left==que[quef].time_exe) 
 //该进程是否已开始  
               {  
 
        que[quef].time_start=time;        //设置开始时间
         que[quef].pro_state=EXECUTE;      //设置状态为执行
                 } 
   que[quef].time_left--;   //剩余执行时间减去时间片1 
 //打印进程信息 
 cout<<" 进程编号: "<<que[quef].pro_id<<"  进程名字: " <<que[quef].pro_name<<"  提交时间: "<<que[quef].time_submit<<" 执行时间: "<<que[quef].time_exe<<"\n 开始时间: "<<que[quef].time_start<<"  还剩时间: "<<que[quef].time_left<<endl;
   //该进程是否执行结束
               if(que[quef].time_left==0)               
               {  
                    que[quef].time_end=time; 
                    que[quef].time_turn= que[quef].time_end-que[quef].time_submit+1; 
                    que[quef].time_aver= que[quef].time_turn/(double)que[quef].time_exe; 
                    que[quef].pro_state=END;   
          cout<<"\n进程"<<que[quef].pro_id<<"  运行结束:"<<"\n 进程名字: " <<que[quef].pro_name<<"  提交时间: "<<que[quef].time_submit<<"  执行时间: " <<que[quef].time_exe<<"   开始时间: "  <<que[quef].time_start<<"   结束时间: "  <<que[quef].time_end<<"\n 周转时间: "<<que[quef].time_turn<<" 带权周转: "<<que[quef].time_aver<<endl ; 
     quef++;         //pcb出队列
 
  
 } 
}  
}                                         // CPU模拟结束
 cout<<"\n所有进程运行完毕！"<<endl;   
double c=(time-freetime)/(double)time*100;  
cout<<"\ncpu利用率："<<c<<"%"<<endl;  
}; 
//主函数
 int main() 
{  
cout<<"***进程调度模拟程序***\n";  
cout<<" -----------------------------------------------------------\n";  
cout<<"*** 专业:信息与计算科学 2012级***\n";  
 cout<<" ***学号: ***\n"; 
 cout<<" *** 姓名:  ***\n"; 
 cout<<"*** 指导老师: ***\n"; 
cout<<"-----------------------------------------------------------\n";  
    cout<<"  \n";  
    cout<<" \n";  
    CpuModel cpu; 
    cpu.cpurun(); 
   return(0);
}
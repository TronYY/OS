#include <iostream.h> 
#include <queue> 
#include <string> 
#include <windows.h>  
#include <malloc.h> 
#include<algorithm>   
#define UNREADY 0        //δ����
#define READY 1          //����
#define EXECUTE 2        //ִ��
#define END 3            //���
using namespace std;  //���̵�״̬
//���̵�PCB�ṹ���Ͷ���ͽṹ����˵��
 struct ProcessPcb 
{  char pro_name[20];    //���̵�����
   int  time_submit;     //�ύʱ��,��ʱ���1��ʼ��ʱ
   int  time_exe;       //�������������ʱ��
   int  pro_id;        //����ID(ϵͳ����) 
   int  pro_priority;    //�������ȼ� 
   int  time_start;     //��ʼִ�е�ʱ��
   int  time_end;        //������ʱ��
   int  time_wait;       //�ȴ���ʱ��
   int  pro_state;       //���̵�״̬ (������ִ�У����)   
   int  time_left;      //�������ʱ�䵥λ,��ʼ��Ϊ�����ִ��ʱ��  
   int  time_turn;    //��תʱ��
   double time_aver;     //��Ȩ��תʱ�� 
   ProcessPcb *next; 
   ProcessPcb() 
      { 
       next=NULL;
      }
}ProcessPcb;  
//��װCPU������������ģ�顣������
  class CpuModel 
{  public: 
  CpuModel() 
  {  
     pcbnum=0; 
  } 
  void cpurun();         //cpuģ�����к���
  bool CreatePcb();         //�������뺯��
  void ShowPcb();        //������Ľ���չʾ����
  void PriModel();       //��ǿռ�����Ƚ��̵���ģʽ
  void FcfsModel();      //�ȵ��ȷ������ģʽ
  private: 
   struct ProcessPcb PcbList[100];     //���ύʱ���ŵ�δ�������̶���
   int pcbnum;                //��������
   int  freetime;             //cpu����ʱ��
   int allturn;                //����תʱ��
   float allaver;              //�ܴ�Ȩ��תʱ��
 }; 
     //cpuģ�����к���
 void CpuModel::cpurun() 
{    int choose=0;
    while(1) 
	{
     cout<<"***���˵�***"<<endl; 
     cout<<"----------------------------------"<<endl; 
     cout<<"***1:��������***"<<endl; 
     cout<<"***2:��ʾ�Ѵ����Ľ���***"<<endl; 
     cout<<"***3:��ǿռ�����Ƚ��̵���***"<<endl; 
     cout<<"**4:�ȵ��ȷ������***"<<endl;  
     cout<<"***5:�˳�ϵͳ***"<<endl; 
     cout<<"\n***��ѡ��: ";
     cin>>choose;  
     cout<<endl; 
     switch(choose)
	 { case 1: CreatePcb();     //��������
           break; 
       case 2: ShowPcb();     //��ʾ�Ѵ����Ľ���
           break; 
       case 3: PriModel();    //��ǿռ�����Ƚ��̷�ʽ
           break;
       case 4: FcfsModel();     //�ȵ��ȷ�����ȷ�ʽ 
           break; 
       case 5: return;          //��������
	 } 
      cout<<endl; 
	} 
};  
 //�������ύʱ������ıȽϺ��� 
bool cmp(struct ProcessPcb a,struct ProcessPcb b) 
{  
     if(a.time_submit<b.time_submit) 
       return true; 
     else return false; 
};  //�ж��Ƿ�����
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
//�������뺯��
 bool CpuModel::CreatePcb() 
{  char num[10]; 
   cout<<"\n***���������뽨���Ľ��̸���: "; 
    cin>>num;    //������̸���
 pcbnum=isnumber(num);
 if(pcbnum<=0)  
    { cout<<"***�����д�***\n"; 
 return 0; 
   }  
  int i;  
  for(i=0;i<pcbnum;i++) //���������Ϣ
 
     { cout<<"***�����������"<<i+1<<"�����̵���Ϣ***"<<endl; 
       cout<<"*******���֣�";
       cin>>PcbList[i].pro_name;
       cout<<"***�ύʱ�䣺";
       cin>>num; 
PcbList[i].time_submit=isnumber(num); 
if(PcbList[i].time_submit<=0) 
             {  
cout<<"***�����д�***\n"; 
return 0; 
              } 
          cout<<"***ִ��ʱ�䣺"; 
            cin>>num;  
            PcbList[i].time_exe=isnumber(num); 
            if(PcbList[i].time_exe<=0) 
{ cout<<"***�����д�***\n";
                            return 0; 
} 
 PcbList[i].time_left=PcbList[i].time_exe;  //����ʣ��ִ��ʱ��  
 PcbList[i].pro_state=UNREADY;        //����״̬Ϊδ����  
PcbList[i].pro_priority=100-PcbList[i].time_exe; //�������ȼ������ȼ�=100-ִ��ʱ��
 cout<<endl;
          } 
 sort(PcbList,PcbList+pcbnum,cmp);        //���ύʱ������
 
           for(i=0;i<pcbnum;i++)  
            PcbList[i].pro_id=i+1;  //����Id,�����ύʱ��  
           return 1; 
};  
//չʾ����ִ�н�����Ϣ 
void CpuModel::ShowPcb() 
{           int i;  
         cout<<"\n***����ִ�еĽ�����Ϣ����***\n"; 
         for(i=0;i<pcbnum;i++)                
cout<<"���̱��:"<<PcbList[i].pro_id<<"��������: "<<PcbList[i].pro_name<<"�ύʱ��:"<<PcbList[i].time_submit<<"ִ��ʱ��: "<<PcbList[i].time_exe<<"���ȼ�����"<<PcbList[i].pro_priority<<endl;
         cout<<endl; 
};  
//��ǿռ�����Ƚ��̵���ģʽ
void CpuModel::PriModel() 
{ cout<<"********��ǿռ�����Ƚ��̵��ȹ�������********\n"; 
    freetime=0;   //��ʼ��ϵͳ����ʱ��Ϊ0 
    int time=0;  //ʱ��
     int nextid=0;  
    int pnum=0;   //�Ѿ���������Ŀ
    struct ProcessPcb *head;  //�������� 
    struct ProcessPcb *pcb,*pcb1,*pcb2; 
    head=(struct ProcessPcb*)malloc(sizeof(struct ProcessPcb));  
    head->next=NULL;       //headָ�벻�ã�head->next��ʼΪ��������
//ģ��cpu��ʼ����
while(head->next!=NULL||pnum<pcbnum) 
   {  
     Sleep(1000);  
     time++;     //ʱ��ƬΪ1  
     cout<<"*Time:��"<<time<<"��"<<endl;     //��ӡ 
// ���̾������鿴δ�����Ľ������ڸ�ʱ��Ƭ�Ƿ���Խ���������У�
    while(pnum<pcbnum && PcbList[pnum].time_submit<=time)  
	{  
     pcb=(struct ProcessPcb*)malloc(sizeof(struct ProcessPcb)); 
     *pcb=PcbList[pnum++]; //�����������
     pcb->next=head->next;        //����head֮�� 
     head->next=pcb; 
	}  //������timeʱ��ƬӦ�����еĽ��� 
      if(head->next==NULL) 
         {  cout<<"û�п����еĽ���"<<endl; 
            freetime++; 
          } 
        else 
         {  
                    pcb2=head;  
           pcb=head->next; 
          pcb1=head->next; //���ҵ�ǰ���ȼ���ߵľ�������
     while(pcb1->next!=NULL) 
         {  
           if(pcb->pro_priority<=pcb1->next->pro_priority) 
               {  pcb2=pcb1;  
                 pcb=pcb1->next; 
               }  
           pcb1=pcb1->next; 
         } //�������ȼ���ߵĽ���
    if(pcb->time_left==pcb->time_exe)    //�ý����Ƿ��ѿ�ʼ
	{  pcb->time_start=time;           //���ÿ�ʼʱ��
     pcb->pro_state=EXECUTE;         //����״̬Ϊִ��
	}  
    pcb->pro_priority=pcb->pro_priority-3;   //���ȼ�����3 
    pcb->time_left--;         //ʣ��ִ��ʱ���ȥʱ��Ƭ1 
   //��ӡ�ý�����Ϣ
    cout<<"���̱��: "<<pcb->pro_id<<"  ��������:" <<pcb->pro_name<<"  �ύʱ��: "<<pcb->time_submit<<"  ִ��ʱ��: "<<pcb->time_exe<<"\n��ʼʱ��: "<<pcb->time_start<<"  ��ʣʱ��: "<<pcb->time_left<<"���ȼ�����"<<pcb->pro_priority<<endl;  //�ý����Ƿ�ִ�н���
   if(pcb->time_left==0)          
   {  pcb2->next=pcb2->next->next;//�����ھ��������г���
        pcb->time_end=time;     //���ý���ʱ��
        pcb->time_turn=pcb->time_end-pcb->time_submit+1; //������תʱ�� 
        pcb->time_aver=pcb->time_turn/(double)pcb->time_exe; //�����Ȩ��תʱ��
          pcb->pro_state=END;  
        cout<<"\n����"<<pcb->pro_id<<"  ���н�����"<<"\n  ��������: "<<pcb->pro_name<<"  �ύʱ��: "<<pcb->time_submit<<"  ִ��ʱ��: "<<pcb->time_exe<<"  ��ʼʱ��: "<<pcb->time_start<<"����ʱ��: "<<pcb->time_end<<"\n��תʱ��: "<<pcb->time_turn<<"��Ȩ��תʱ��: "<<pcb->time_aver<<endl;  
   }
		} 
}   
                                  
//ģ��CPU��������
  cout<<"\n ���н���������ϣ�"<<endl;
        double c=(time-freetime)/(double)time*100; 
      cout<<"\n cpu�����ʣ�"<<c<<"%"<<endl;  
};                              
//��ǿռ�����Ƚ��̵���ģʽ���� 
//�ȵ��ȷ������ģʽ 
void CpuModel::FcfsModel() 
{  
   cout<<"********�ȵ��ȷ�����̵��ȹ�������********\n"; 
     freetime=0;             //��ʼ��ϵͳ����ʱ��Ϊ0 
      int   time=0;           //ʱ��  
     int   pnum=0;           //�Ѿ���������Ŀ
 
     struct ProcessPcb que[100];    //��������
       int   quef=0,quee=0;     //�������е�ͷָ���βָ��
  //ģ��cpu��ʼ����
 while(quef<quee||pnum<pcbnum) 
     {  
           Sleep(1000);  
           time++;//ʱ��ƬΪ1  
           cout<<"*Time: ��"<<time<<"��"<<endl;    //��ӡ
    //���̾������鿴δ�����Ľ������ڸ�ʱ��Ƭ�Ƿ���Խ���������У�
    while(pnum<pcbnum && PcbList[pnum].time_submit<=time)  
que[quee++]=PcbList[pnum++]; 
    //����timeʱ��Ƭ�����н���
 
           if(quef==quee) 
           {  cout<<"û�п����еĽ���"<<endl; 
                freetime++; 
           } 
           else  
           {                 
    //���н���
            if(que[quef].time_left==que[quef].time_exe) 
 //�ý����Ƿ��ѿ�ʼ  
               {  
 
        que[quef].time_start=time;        //���ÿ�ʼʱ��
         que[quef].pro_state=EXECUTE;      //����״̬Ϊִ��
                 } 
   que[quef].time_left--;   //ʣ��ִ��ʱ���ȥʱ��Ƭ1 
 //��ӡ������Ϣ 
 cout<<" ���̱��: "<<que[quef].pro_id<<"  ��������: " <<que[quef].pro_name<<"  �ύʱ��: "<<que[quef].time_submit<<" ִ��ʱ��: "<<que[quef].time_exe<<"\n ��ʼʱ��: "<<que[quef].time_start<<"  ��ʣʱ��: "<<que[quef].time_left<<endl;
   //�ý����Ƿ�ִ�н���
               if(que[quef].time_left==0)               
               {  
                    que[quef].time_end=time; 
                    que[quef].time_turn= que[quef].time_end-que[quef].time_submit+1; 
                    que[quef].time_aver= que[quef].time_turn/(double)que[quef].time_exe; 
                    que[quef].pro_state=END;   
          cout<<"\n����"<<que[quef].pro_id<<"  ���н���:"<<"\n ��������: " <<que[quef].pro_name<<"  �ύʱ��: "<<que[quef].time_submit<<"  ִ��ʱ��: " <<que[quef].time_exe<<"   ��ʼʱ��: "  <<que[quef].time_start<<"   ����ʱ��: "  <<que[quef].time_end<<"\n ��תʱ��: "<<que[quef].time_turn<<" ��Ȩ��ת: "<<que[quef].time_aver<<endl ; 
     quef++;         //pcb������
 
  
 } 
}  
}                                         // CPUģ�����
 cout<<"\n���н���������ϣ�"<<endl;   
double c=(time-freetime)/(double)time*100;  
cout<<"\ncpu�����ʣ�"<<c<<"%"<<endl;  
}; 
//������
 int main() 
{  
cout<<"***���̵���ģ�����***\n";  
cout<<" -----------------------------------------------------------\n";  
cout<<"*** רҵ:��Ϣ������ѧ 2012��***\n";  
 cout<<" ***ѧ��: ***\n"; 
 cout<<" *** ����:  ***\n"; 
 cout<<"*** ָ����ʦ: ***\n"; 
cout<<"-----------------------------------------------------------\n";  
    cout<<"  \n";  
    cout<<" \n";  
    CpuModel cpu; 
    cpu.cpurun(); 
   return(0);
}
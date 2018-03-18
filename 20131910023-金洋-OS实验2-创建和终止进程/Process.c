	
#include <stdio.h>
#include <signal.h>//ͷ�ļ�<signal.h>���ṩ��һЩ���ڴ�����������ڼ����������쳣�����Ĺ��ܣ��紦����Դ���ⲿ���ж��źŻ����ִ���ڼ���ֵĴ�����¼�
#include <unistd.h>
#include <sys/types.h>
int wait_flag;//ȫ�ֱ���wait_flag,������ʶ����״̬

void stop(){
	wait_flag=0;
}

void main(){
	int pid1,pid2;//�����������̺Ų���
	signal(3,stop); //�� signal(14,stop);signal()����������ý��̿������ж��źŵĴ���

	while ( (pid1=fork())==-1);//����ȴ� �ɹ������ӽ��̵��¼�����
	
	if (pid1>0){
	// �ӽ��̴����ɹ�,pid1Ϊ���̺�
	
		while  ((pid2=fork())==-1);	// �����ӽ���2  
		if (pid2>0){
			wait_flag=1;//���Ϊ����״̬
			sleep(5);// �����̵ȴ�5��
			kill(pid1,16);//ɱ������1 
			kill(pid2,17);//ɱ������2
			wait(0);//�ȴ��ӽ���1�������ź�
			wait(0);//�ȴ��ӽ���2�������ź�
			printf("\n Parent process is killed!!\n");
			exit(0); // �����̽���
		}
		else{
			wait_flag=1;
			signal(17,stop);//�ȴ�����2��ɱ�����жϺ�17
			printf("\n Child process 2 is killed by parent!!\n");
			exit(0);
		}
	
	else{
		wait_flag=1;
		signal(16,stop);//�ȴ�����1��ɱ�����жϺ�16
		printf("\n Child process 1 is killed by parent!!\n");
		exit(0);
	}

}
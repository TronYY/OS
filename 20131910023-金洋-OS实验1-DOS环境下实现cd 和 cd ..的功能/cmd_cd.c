#define BUFSIZE 100  /*BUFSIZE������ܹ�����ĵ�ַ����*/
#define HISNUM 12
char buf[BUFSIZE];
typedef struct ENV_HISTORY{
	int start;
	int end;
	char his_cmd[HISNUM][100];
}	ENV_HISTORY;//����ṹ������ ENV_HISTORY������typedef������������ΪENV_HISTORY

ENV_HISTORY envhis;
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<winbase.h>
#include<wincon.h>
#include<tlhelp32.h>
#include<malloc.h>
#include<string.h>
#include<direct.h>

void main()
{     
	/***************************��������ĺ���*************************************/
	void add_history(char *inputcmd);//�洢��ʷ·������
	void cd_cmd(char *route);/����·������
	/******************************��������ĺ���************************************/

	char c,*input,*arg[2],path[BUFSIZE];
	int input_len=0, is_bg=0,i,j,k;
 
	DWORD dwRet;//DWORDһ���������ͣ�һ��32λ�޷���������һ�μ�����صĵ�ַƫ����
	/******************************ִ������*********************************************/
	while(1)//ʹ��������������ִ��
	{
		/*��ָ�����������ָ�������ʼ��*/
		for(i=0;i<2;i++)
			arg[i]=NULL;

		/*��õ�ǰĿ¼�����صĵ�ַ����"path"��*/
		/*BUFSIZE������ܹ�����ĵ�ַ����*/
		dwRet=GetCurrentDirectory(BUFSIZE,path);//��ȡ��ǰ����·��
	
		if(dwRet==0)
		{
			/*���ص�ǰĿ¼ʧ�ܣ����������Ϣ*/
			printf("GetCurrentDirectory failed (%d)\n",GetLastError());
		}

		else if(dwRet>BUFSIZE)
		{
		/*BUFSIZE����С�ڷ��ص�ַ���ȣ�������Ҫ���ٳ���*/
			printf("GetCurrentDirectory failed (buffer to small;need %d char)\n",dwRet);

		}
		else
		{
			/*�����ǰĿ¼*/
			printf("%s>",path);
		}


		/**************************************************����**********************************/
		input_len=0;

		/*�������ַ����˵�*/
		while((c=getchar())==' '||c=='\t'||c==EOF||c=='\n')
		/*	if(c=='\n') ����Ϊ��ʱ����������ѭ����ӡ��ʾ��
				continue*/; 
/*Դ���� ����һ�䲢û��ע�͵�������������ַ�Ϊ�س�ʱ ��������ʽ���һ��ע�ͣ�����while�����ж��м���||c=='\n'*/

		/*���������buf���飬input_len ��¼���������*/
		while(c!='\n')
		{
			buf[input_len++]=c;
			c=getchar();
		}
		buf[input_len++]='\0'; /*�����ַ���������*/

		/*���䶯̬�洢�ռ䣬������ӻ��渴�Ƶ�input��*/
		input=(char *) malloc(sizeof(char)*(input_len));
		strcpy(input,buf);
		/*************************************************�������*************************/



		/*************************����ָ��***************/
		for(i=0,j=0,k=0;i<input_len;i++)
		{
			if(input[i]==' '||input[j]=='\0')
			{
				if(j==0)   /*ȥ������һ��Ķ���ո�*/
				continue;

				else
				{
					buf[j++]='\0';
					arg[k]=(char*)malloc(sizeof(char)*j);
					strcpy(arg[k++],buf);/*��ָ���������Ƶ�arg��*/
					j=0;       /*׼��ȡ��һ������*/
				}
			}
			else
			{
				buf[j++]=input[i];
			}
		}

		/*********************************����ָ�����************************/


		/*cd����*/
		if(strcmp(arg[0],"cd")==0)
		{
			add_history(input);  /*�������������ӵ���ʷ������*/
			for(i=3,j=0;i<=input_len;i++)/*��ȡcd�������ز���*/
			buf[j++]=input[i];
			buf[j]='\0';
			arg[1]=(char*)malloc(sizeof(char)*j);
			strcpy(arg[1],buf);
			cd_cmd(arg[1]); /*��ʾcd����*/
			free(input);
			continue;
		}
		/*****************************cd����ִ�н���********************/
		/*dir����*/

	}
 /******************************ִ���������********************************/
}


void cd_cmd(char *route)
{
	if(!SetCurrentDirectory(route)) /*���õ�ǰĿ¼����ʧ���򷵻س�����Ϣ*/
	{
	printf(TEXT("SetCurrentDirectory failed (%d)\n"),GetLastError());
	}
}

void add_history(char *inputcmd)
{
	envhis.end=(envhis.end+1)%HISNUM;
	/*end��startָ��ͬһ����*/
	if(envhis.end==envhis.start)
	{
		/*startǰ��һλ*/
		envhis.start=(envhis.start+1)%HISNUM;
	}
	/*���������endָ���������*/
	strcpy(envhis.his_cmd[envhis.end],inputcmd);
}

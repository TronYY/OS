#define BUFSIZE 100  /*BUFSIZE是最多能够保存的地址长度*/
#define HISNUM 12
char buf[BUFSIZE];
typedef struct ENV_HISTORY{
	int start;
	int end;
	char his_cmd[HISNUM][100];
}	ENV_HISTORY;//定义结构体类型 ENV_HISTORY，并用typedef声明其类型名为ENV_HISTORY

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
	/***************************声明所需的函数*************************************/
	void add_history(char *inputcmd);//存储历史路径函数
	void cd_cmd(char *route);/更改路径函数
	/******************************声明所需的函数************************************/

	char c,*input,*arg[2],path[BUFSIZE];
	int input_len=0, is_bg=0,i,j,k;
 
	DWORD dwRet;//DWORD一种数据类型，一个32位无符号整数或一段及其相关的地址偏移量
	/******************************执行命令*********************************************/
	while(1)//使程序不跳出，持续执行
	{
		/*将指向输入命令的指针数组初始化*/
		for(i=0;i<2;i++)
			arg[i]=NULL;

		/*获得当前目录，返回的地址存入"path"中*/
		/*BUFSIZE是最多能够保存的地址长度*/
		dwRet=GetCurrentDirectory(BUFSIZE,path);//获取当前磁盘路径
	
		if(dwRet==0)
		{
			/*返回当前目录失败，输出出错信息*/
			printf("GetCurrentDirectory failed (%d)\n",GetLastError());
		}

		else if(dwRet>BUFSIZE)
		{
		/*BUFSIZE长度小于返回地址长度，输入需要多少长度*/
			printf("GetCurrentDirectory failed (buffer to small;need %d char)\n",dwRet);

		}
		else
		{
			/*输出当前目录*/
			printf("%s>",path);
		}


		/**************************************************输入**********************************/
		input_len=0;

		/*将无用字符过滤掉*/
		while((c=getchar())==' '||c=='\t'||c==EOF||c=='\n')
		/*	if(c=='\n') 输入为空时，结束本次循环打印提示符
				continue*/; 
/*源代码 中上一句并没有注释掉，易造成无用字符为回车时 程序出错，故将上一句注释，并在while语句的判断中加入||c=='\n'*/

		/*将命令存入buf数组，input_len 记录输入命令长度*/
		while(c!='\n')
		{
			buf[input_len++]=c;
			c=getchar();
		}
		buf[input_len++]='\0'; /*加上字符串结束符*/

		/*分配动态存储空间，将命令从缓存复制到input中*/
		input=(char *) malloc(sizeof(char)*(input_len));
		strcpy(input,buf);
		/*************************************************输入结束*************************/



		/*************************解析指令***************/
		for(i=0,j=0,k=0;i<input_len;i++)
		{
			if(input[i]==' '||input[j]=='\0')
			{
				if(j==0)   /*去掉连在一起的多个空格*/
				continue;

				else
				{
					buf[j++]='\0';
					arg[k]=(char*)malloc(sizeof(char)*j);
					strcpy(arg[k++],buf);/*将指令或参数复制到arg中*/
					j=0;       /*准备取下一个参数*/
				}
			}
			else
			{
				buf[j++]=input[i];
			}
		}

		/*********************************解析指令结束************************/


		/*cd命令*/
		if(strcmp(arg[0],"cd")==0)
		{
			add_history(input);  /*将输入的命令添加到历史命令中*/
			for(i=3,j=0;i<=input_len;i++)/*获取cd命令的相关参数*/
			buf[j++]=input[i];
			buf[j]='\0';
			arg[1]=(char*)malloc(sizeof(char)*j);
			strcpy(arg[1],buf);
			cd_cmd(arg[1]); /*显示cd命令*/
			free(input);
			continue;
		}
		/*****************************cd命令执行结束********************/
		/*dir命令*/

	}
 /******************************执行命令结束********************************/
}


void cd_cmd(char *route)
{
	if(!SetCurrentDirectory(route)) /*设置当前目录，若失败则返回出错信息*/
	{
	printf(TEXT("SetCurrentDirectory failed (%d)\n"),GetLastError());
	}
}

void add_history(char *inputcmd)
{
	envhis.end=(envhis.end+1)%HISNUM;
	/*end和start指向同一数组*/
	if(envhis.end==envhis.start)
	{
		/*start前移一位*/
		envhis.start=(envhis.start+1)%HISNUM;
	}
	/*将命令存入end指向的数组中*/
	strcpy(envhis.his_cmd[envhis.end],inputcmd);
}

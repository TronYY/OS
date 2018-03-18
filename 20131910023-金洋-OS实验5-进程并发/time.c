#include <stdio.h>
#include <windows.h>
#include <time.h>//获取时间的头文件
#include <conio.h>
void main()
{
	SYSTEMTIME currentTime;
    GetSystemTime(&currentTime);
    printf("time: %u/%u/%u %u:%u:%u:%u %d\n",           
     currentTime.wYear,currentTime.wMonth,currentTime.wDay,
     currentTime.wHour,currentTime.wMinute,currentTime.wSecond,
     currentTime.wMilliseconds,currentTime.wDayOfWeek);
    


}
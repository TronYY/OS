#include <stdio.h>
#include <windows.h>
#include <time.h>
#include <iostream.h>

void main(){
	int i;
	srand((unsigned) time(NULL));
	for (i=0;i<20;i++) print("%d\n",rand()%101);


}
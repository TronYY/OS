#include <stdio.h>
#include <windows.h>
#include <iostream.h>

int buffer[3]={0,0,0};

void display(){
	int i;
	printf("buffers' status:");
	for (i=0;i<3;i++) printf(" %d",buffer[i]);
	printf(";\n");


}
DWORD WINAPI producer(){
	int i;
	while (1){
		for (i=0;i<3;i++){
			if (buffer[i]==0){
				buffer[i]++;
				
				printf("Produce an item to buffer %d,",i);
				display();
				break;
			}
			
		}
		Sleep(100);

	}	


}

DWORD WINAPI consumer(){
	int i;
	while (1){
		for (i=0;i<3;i++){
			if (buffer[i]==1){
				buffer[i]--;
				printf("Consume an item from buffer %d,",i);
				display();
				break;

			}
			
		}
		Sleep(100);

	}

}


void main(){
	HANDLE handle[5];
	DWORD dw1,dw2,dw3,dw4,dw5;
	handle[0]=CreateThread(NULL,0,producer,NULL,0,&dw1);
	handle[1]=CreateThread(NULL,0,producer,NULL,0,&dw2);
	handle[2]=CreateThread(NULL,0,producer,NULL,0,&dw3);
	handle[3]=CreateThread(NULL,0,consumer,NULL,0,&dw4);
	handle[4]=CreateThread(NULL,0,consumer,NULL,0,&dw5);
	display();
}

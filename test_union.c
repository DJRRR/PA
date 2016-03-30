#include<stdio.h>

typedef struct{
	union{
		unsigned int first:16;
		unsigned int second:16;
	};
	unsigned int val:32;
}C;
C a;
int main(){
	a.val=0xffff0000;
	printf("first:0x%x\n",a.first);
	printf("second:0x%x\n",a.second);
	return 0;
}

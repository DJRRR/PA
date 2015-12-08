#include "trap.h"
char str[]="abcdefg";
int main(){
	int i=0;
	for(i=0;i<5;i++){
		nemu_assert(str[i]==('a'+i));
	}
	HIT_GOOD_TRAP;
	return 0;
}

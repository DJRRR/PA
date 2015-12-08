#include "trap.h"
char str[7]={'a','b','c','d','e','f','g'};
int main(){
	int i=0;
	int ans=0;
	for(i=0;i<5;i++){
		ans += str[i]-'a';
	}
	HIT_GOOD_TRAP;
	return 0;
}

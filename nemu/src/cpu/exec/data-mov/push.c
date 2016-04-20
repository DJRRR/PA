#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "push-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "push-template.h"
#undef DATA_BYTE

make_helper_v(push_r)
make_helper_v(push_rm)
make_helper_v(push_i)

void push_l(uint32_t x){
	cpu.esp -=4;
	swaddr_write(cpu.esp,4,x,S_SS);
}

make_helper(pusha){
	swaddr_t temp=cpu.esp;
	int i;
	for(i=R_EAX;i<=R_EDI;i++){
		if(i==R_ESP) push_l(temp);
		else push_l(reg_l(i));
	}
	print_asm("pusha");
	return 1;
}

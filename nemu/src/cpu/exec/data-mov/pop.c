#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "pop-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "pop-template.h"
#undef DATA_BYTE

make_helper_v(pop_rm)
make_helper_v(pop_r)

uint32_t pop_l(){
	uint32_t re=swaddr_read(cpu.esp,4,S_SS);
	cpu.esp += 4;
	return re;
}

make_helper(popa){
	int i;
	for(i=R_EDI;i>=R_EAX;i--){
		if(i==R_ESP) pop_l();
		else reg_l(i)=pop_l();
	}
	print_asm("popa");
	return 1;
}


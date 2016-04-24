#include "cpu/exec/template-start.h"

#define instr stos
make_helper(concat(stos_m_,SUFFIX)){
	int current_sreg=S_ES;
	uint32_t dest=cpu.edi;
	if(DATA_BYTE==1){
		MEM_W(dest,REG(R_AL));
		if(cpu.DF==0){
			dest++;
		}
		else{
			dest--;
		}
	}
	else if(DATA_BYTE==2){
		MEM_W(dest,REG(R_AX));
		if(cpu.DF==0){
			dest += 2;
		}
		else{
			dest -= 2;
		}
	}
	else{
		MEM_W(dest,REG(R_EAX));
		if(cpu.DF==0){
			dest += 4;
		}
		else{
			dest -= 4;
		}
	}
	cpu.edi=dest;
	print_asm("stos %%esi,%%edi\n");
	return 1;
}

#include "cpu/exec/template-end.h"

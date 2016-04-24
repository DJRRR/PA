#include "cpu/exec/template-start.h"

#define instr stos
make_helper(concat(stos_m_,SUFFIX)){
	int current_sreg=S_ES;
	if(DATA_BYTE==1){
		MEM_W(cpu.edi,cpu.gpr[0]._8[0]);
		if(cpu.DF==0){
			cpu.edi++;
		}
		else{
			cpu.edi--;
		}
	}
	else if(DATA_BYTE==2){
		MEM_W(cpu.edi,cpu.gpr[0]._16);
		if(cpu.DF==0){
			cpu.edi += 2;
		}
		else{
			cpu.edi -= 2;
		}
	}
	else{
		MEM_W(cpu.edi,cpu.eax);
		if(cpu.DF==0){
			cpu.edi += 4;
		}
		else{
			cpu.edi -= 4;
		}
	}
	print_asm_template1();
	return DATA_BYTE;
}

#include "cpu/exec/template-end.h"

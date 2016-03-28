#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_m_,SUFFIX)){
	int current_sreg=2;
	MEM_W(cpu.edi,swaddr_read(cpu.esi,DATA_BYTE,S_DS));
	if(cpu.DF==0){
		cpu.edi += DATA_BYTE;
		cpu.esi += DATA_BYTE;
	}
	else{
		cpu.edi -= DATA_BYTE;
		cpu.esi -= DATA_BYTE;
	}
	print_asm_template2();
	return DATA_BYTE;
}

#include "cpu/exec/template-end.h"

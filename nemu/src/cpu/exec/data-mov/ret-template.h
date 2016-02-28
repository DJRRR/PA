#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_,SUFFIX)){
	cpu.eip = swaddr_read(cpu.esp,4)-1;
	if(DATA_BYTE==2){
		cpu.esp += 2;
	}
	else{
		cpu.esp += 4;
	}
	if(DATA_BYTE==2){
		cpu.eip = cpu.eip&0x0000ffff;
	}
	print_asm("ret");
	return 1;
}

make_helper(concat(ret_imm_,SUFFIX)){
	swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	cpu.eip=swaddr_read(cpu.esp,4)-3;
	cpu.esp += addr;
	if(DATA_BYTE==2){
		cpu.eip = cpu.eip &0x0000ffff;
	}
	print_asm_template1();
	return 3;
}

#include "cpu/exec/template-end.h"

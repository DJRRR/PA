#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
	printf("0x%X\n",instr_fetch(cpu.eip+3,4));
	hwaddr_t addr=instr_fetch(cpu.eip+3,4);
	printf("0x%X\n",addr);
	if(ops_decoded.is_data_size_16){
		cpu.gdtr.bound=swaddr_read(addr,2);
		cpu.gdtr.base=swaddr_read(addr+2,3);


	}
	else{
	}
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

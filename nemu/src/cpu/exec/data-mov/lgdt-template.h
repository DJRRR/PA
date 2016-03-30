#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
//	printf("0x%X\n",instr_fetch(cpu.eip+3,4));
	hwaddr_t addr=instr_fetch(cpu.eip+3,4);
	uint32_t judge=instr_fetch(cpu.eip+2,1);
	printf("!!!!!!!!!%.2X\n",judge);
//	printf("0x%X\n",addr);
	if(ops_decoded.is_data_size_16){
		cpu.gdtr.limit=swaddr_read(addr,2,S_CS)&0x0000ffff;
		cpu.gdtr.base=swaddr_read(addr+2,3,S_CS)&0x00ffffff;
	}
	else{
		cpu.gdtr.limit=swaddr_read(addr,2,S_CS)&0x0000ffff;
		cpu.gdtr.base=swaddr_read(addr+2,4,S_CS);
	}
	print_asm_template1();
}

make_instr_helper(rm);

#include "cpu/exec/template-end.h"

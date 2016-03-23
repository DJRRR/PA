#include "cpu/exec/template-start.h"

#define instr lidt

static void do_execute(){
	hwaddr_t addr=instr_fetch(cpu.eip+3,4);
	if(ops_decoded.is_data_size_16){
		cpu.idtr.bound=swaddr_read(addr,2)&0x0000ffff;
		cpu.idtr.base=swaddr_read(addr+2,3)&0x00ffffff;
	}
	else{
		cpu.idtr.bound=swaddr_read(addr,2)&0x0000ffff;
		cpu.idtr.base=swaddr_read(addr+2,4);
	}
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
	printf("0x%X\n",instr_fetch(cpu.eip+3,4));
	if(ops_decoded.is_data_size_16){


	}
	else{
	}
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute(){

	printf("%X\n",instr_fetch(cpu.eip+2,1));
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

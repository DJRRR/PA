#include "cpu/exec/template-start.h"

#define instr lidt

static void do_execute(){
	printf("0x%.2X\n",instr_fetch(cpu.eip+1,1));
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	cpu.esp -= 4;
	MEM_W(cpu.esp,op_src->val);
	print_asm_template1();
	cpu.eip += op_src->val+5;
	op_src->val = cpu.eip;
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"


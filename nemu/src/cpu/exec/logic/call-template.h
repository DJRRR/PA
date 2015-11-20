#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	cpu.esp -= 4;
	MEM_W(cpu.esp,cpu.eip+1+DATA_BYTE);
	cpu.eip += op_src->val;
//	op_src->val = cpu.eip;
//	printf("op_src->val : %u\n",op_src->val);
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"


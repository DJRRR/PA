#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	cpu.esp -= 4;
	op_dest->type=OP_TYPE_MEM;
    op_dest->addr=cpu.esp;
//	MEM_W(cpu.esp,cpu.eip);
//	cpu.eip += op_src->val;
	OPERAND_W(op_dest,cpu.eip+5);
	cpu.eip += (DATA_TYPE_S) op_src->val;
    op_src->val=cpu.eip;
	printf("%0X\n",cpu.eip+5);
    print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"


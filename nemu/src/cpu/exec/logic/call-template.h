#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	MEM_W(cpu.esp,cpu.eip);
	cpu.eip += op_dest->val;
    print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"


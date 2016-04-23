#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
	int n=4;
	if(ops_decoded.is_data_size_16) n=2;
	cpu.esp-=n;
	swaddr_write(cpu.esp,n,op_src->val,S_SS);
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)
make_instr_helper(i)

#include "cpu/exec/template-end.h"

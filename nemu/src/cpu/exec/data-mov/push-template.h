#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
	int n=4;
	int current_sreg=S_SS;
	if(DATA_BYTE==2) n=2;
	cpu.esp -= n;
	MEM_W(cpu.esp,op_src->val);
	//swaddr_write(cpu.esp,n,op_src->val,S_SS);
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)
make_instr_helper(i)

#include "cpu/exec/template-end.h"

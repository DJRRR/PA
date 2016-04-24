#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	int n=4;
	if(ops_decoded.is_data_size_16) n=2;
	int current_sreg=S_SS;
	OPERAND_W(op_src,MEM_R(cpu.esp));//unchecked
	cpu.esp += n;
    print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)


#include "cpu/exec/template-end.h"

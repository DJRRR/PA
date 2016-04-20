#include "cpu/exec/template-start.h"

#define instr jb

static void do_execute(){
	if(cpu.CF==1){
		cpu.eip += (DATA_TYPE_S)op_src->val;
		if(ops_decoded.is_data_size_16){
			cpu.eip=cpu.eip & 0x0000ffff;
		}
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"


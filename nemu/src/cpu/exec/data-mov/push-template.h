#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
//	cpu.esp -= 4;
//	MEM_W(cpu.esp,op_src->val);
//	print_asm_template1();
	if(ops_decoded.is_data_size_16){
		cpu.esp -= 2;
		MEM_W(cpu.esp,op_src->val);
	}
	else{
		cpu.esp -= 4;
		 MEM_W(cpu.esp,op_src->val);
	}
	print_asm_template1();

}

make_instr_helper(r)
make_instr_helper(rm)
make_instr_helper(i)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr push

static void do_execute(){
/*//	cpu.esp -= 4;
//	MEM_W(cpu.esp,op_src->val);
//	print_asm_template1();
	if(ops_decoded.is_data_size_16){
		cpu.gpr[4]._16 -= 2;
		MEM_W(cpu.gpr[4]._16,(op_src->val)&0xffff);
	}
	else{
		cpu.esp -= 4;
	 MEM_W(cpu.esp,op_src->val);
	   //  swaddr_write(cpu.esp,4,op_src->val);
	}*/
	uint32_t n=4;
	if(ops_decoded.is_data_size_16) n=2;
	cpu.esp -= n;
	swaddr_write(cpu.esp,n,op_src->val);
	print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)
make_instr_helper(i)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
	DATA_TYPE result=op_dest->val & op_src->val;
	cpu.OF=0;
	cpu.CF=0;
	unsigned int flag_res=MSB(result)&1;
	cpu.ZF=!result;
	cpu.SF=flag_res;
	DATA_TYPE res_t=result;
	res_t = res_t &0xff;
	res_t ^= res_t>>4;
	res_t ^= res_t>>2;
	res_t ^= res_t>>1;
	cpu.PF=!res_t;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"





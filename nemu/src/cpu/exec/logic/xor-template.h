#include "cpu/exec/template-start.h"

#define instr xor

static void do_execute () {
	DATA_TYPE result = op_dest->val ^ op_src->val;
	OPERAND_W(op_dest, result);
	DATA_TYPE flag_res=MSB(result)&1;
	cpu.CF=0;
	cpu.OF=0;
	cpu.ZF=!result;
	DATA_TYPE res_t=result;
	res_t = res_t &0xff;
	res_t ^=res_t >>4;
	res_t ^=res_t>>2;
	res_t ^=res_t>>1;
	cpu.PF=!res_t;
	cpu.SF=flag_res;

	/* TODO: Update EFLAGS. */
	

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

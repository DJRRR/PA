#include "cpu/exec/template-start.h"

#define instr inc

static void do_execute () {
	DATA_TYPE result = op_src->val + 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
//	DATA_TYPE flag_dest=MSB(op_dest->val)&1;
//	DATA_TYPE flag_src=MSB(op_src->val)&1;
	DATA_TYPE flag_res=MSB(result)&1;
	cpu.ZF=!result;
	cpu.SF=flag_res;
	cpu.OF=(result==(1<<(DATA_BYTE*8-1)));
	cpu.CF=!result;

	DATA_TYPE res_t = result;
	res_t = res_t &0xff;
	res_t ^= res_t >> 4;
	res_t ^= res_t >> 2;
	res_t ^= res_t >>1 ;
	cpu.PF=!res_t;
	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"

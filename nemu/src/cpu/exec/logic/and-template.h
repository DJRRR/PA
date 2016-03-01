#include "cpu/exec/template-start.h"

#define instr and

static void do_execute () {
	DATA_TYPE result = op_dest->val & op_src->val;
	OPERAND_W(op_dest, result);
	DATA_TYPE result1 = result;
	DATA_TYPE flag_res=MSB((DATA_TYPE_S)result)&1;
	unsigned int num=0;
	int i=0;
	/* TODO: Update EFLAGS. */
	cpu.CF=0;
	cpu.OF=0;
	cpu.ZF=!result;
	for(i=0;i<8;i++){
		if(result1&1){
			num++;
		}
		result1 >>= 1;
	}
	cpu.PF=!(num%2);
	cpu.SF=flag_res;
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

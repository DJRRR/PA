#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

	/* TODO: Update EFLAGS. */
	DATA_TYPE flag_src=MSB(op_src->val)&1;
	DATA_TYPE flag_res=MSB(result)&1;
	unsigned int num=0;
	int i=0;
	cpu.ZF=!result;
	cpu.SF=flag_res;
	if(flag_src==1&&flag_res==0){
		cpu.OF=1;
	}
	else{
		cpu.OF=0;
	}
	if(op_src->val<1){
		cpu.CF=1;
	}
	else{
		cpu.CF=0;
	}
	for(i=0;i<8;i++){
		if(result&1){
			num++;
		}
		result >>= 1;
	}
	cpu.PF=!(num%2);

	print_asm_template1();
}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"

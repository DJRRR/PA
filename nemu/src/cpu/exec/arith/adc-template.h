#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
	uint64_t result1=(uint64_t)op_dest->val+op_src->val+cpu.CF;
	DATA_TYPE result=result1;
//	DATA_TYPE flag_dest=MSB(op_dest->val)&1;
//	DATA_TYPE flag_src=MSB(op_src->val)&1;
	DATA_TYPE flag_res=MSB(result)&1;
	OPERAND_W(op_dest,result);
//	unsigned int num=0;
//	int i=0;
	cpu.SF=flag_res;
	cpu.ZF=!result;
	cpu.CF=(result1>>(DATA_BYTE<<3))&1;
	cpu.OF=(MSB(op_dest->val)^MSB(result))&&!(MSB(op_dest->val)^MSB(op_src->val));
/*	for(i=0;i<8;i++){
		if(result&1){
			num++;
		}
		result >>= 1;
	}
	cpu.PF=!(num%2);*/
	DATA_TYPE res_t=result;
	res_t = res_t &0xff;
	res_t ^= res_t >>4;
	res_t ^= res_t>>2;
	res_t ^= res_t>>1;
	cpu.PF=!res_t;
	print_asm_template2();

}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

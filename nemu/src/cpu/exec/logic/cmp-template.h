#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	DATA_TYPE_S result= op_dest->val - (DATA_TYPE_S)op_src->val;
	DATA_TYPE flag_dest=MSB(op_dest->val)&1;
	DATA_TYPE flag_src=MSB(op_src->val)&1;
	DATA_TYPE flag_res=MSB(result)&1;
	cpu.ZF=!result;
	cpu.SF=flag_res;
	if((flag_dest==1&&flag_src==0&&flag_res==0)||(flag_dest==0&&flag_src==1&&flag_res==1)){
		cpu.OF=1;
	}
	else{
		cpu.OF=0;
	}
	if((DATA_TYPE)op_src->val>(DATA_TYPE)op_dest->val){
		cpu.CF=1;
	}
	else{
		cpu.CF=0;
	}
	DATA_TYPE res_t=result;
	res_t = res_t &0xff;
	res_t ^= res_t>>4;
	res_t ^= res_t >>2;
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

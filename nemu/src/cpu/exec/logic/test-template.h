#include "cpu/exec/template-start.h"

#define instr test

static void do_execute(){
	DATA_TYPE result=op_dest->val & op_src->val;
	cpu.OF=0;
	cpu.CF=0;
	unsigned int flag_res=MSB(result)&1;
//	unsigned int num=0;
//	int i=0;
/*	if(result==0){
		cpu.ZF=1;
	}
	else{
		cpu.ZF=0;
	}*/
	cpu.ZF=!result;
/*	if(flag_res==1){
		cpu.SF=1;
	}
	else{
		cpu.SF=0;
	}*/
	cpu.SF=flag_res;
/*	for(i=0;i<8;i++){
		if((result&1)==1){
			num++;
		}
		result >>= 1;
	}
	if(num%2==0){
		cpu.PF=1;
	}
	else{
		cpu.PF=0;
	}*/
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





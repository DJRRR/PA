#include "cpu/exec/template-start.h"

#define instr movzx

static void do_execute(){
/*	DATA_TYPE res;
	if(instr_fetch(cpu.eip+1,1)==0xb6){
		res=op_src->val;
		res=(res<<24)>>24;
		res = res&0x000000ff;
		OPERAND_W(op_dest,res);
	}
	else if(instr_fetch(cpu.eip+1,1)==0xb7){
		res=op_src->val;
		res = (res<<16)>>16;
		res = res&0x0000ffff;
		OPERAND_W(op_dest,res);
	}*/
	int len=DATA_BYTE<<3;
	uint32_t temp=0,result=op_src->val;
	int i=0;
	for(;i<len;i++){
		temp = temp | (1<<i);
	}
	result = result &temp;
	write_operand_l(op_dest,result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

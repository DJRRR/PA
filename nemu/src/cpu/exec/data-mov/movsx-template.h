#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
/*	DATA_TYPE_S addr=op_src->val;
	swaddr_t opcode=instr_fetch(cpu.eip+1,1);
	if(opcode==0xbe){
		addr=(addr<<24)>>24;
		printf("test1\n");
	}
	else if(opcode==0xbf){
		addr=(addr<<16)>>16;
		printf("test2\n");
	}
	OPERAND_W(op_dest,addr);*/
	int len=DATA_BYTE<<3;
	int32_t result=op_src->val;
	int first=0;
	if((DATA_TYPE_S)op_src->val<0){
		first=1;
	}
	int i=len;
	for(;i<8*sizeof(op_dest->val);i++){
		result = result |(first<<i);
	}
	write_operand_l(op_dest,result);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr sbb

static void do_execute(){
	DATA_TYPE src=op_src->val;
	if(ops_decoded.opcode==0x83){
		src=(DATA_TYPE_S)op_src->val;
	}
	DATA_TYPE result=op_dest->val-(src+cpu.CF);
	OPERAND_W(op_dest,result);
	DATA_TYPE temp=result;
	DATA_TYPE flag_dest=MSB(op_dest->val)&1;
	DATA_TYPE flag_src=MSB(src)&1;
	DATA_TYPE flag_res=MSB(result)&1;
	unsigned int num=0;
	int i=0;
	cpu.SF=flag_res;
	cpu.ZF=!result;
	for(i=0;i<8;i++){
		if(temp&1){
			num++;
		}
		temp >>= 1;
	}
	cpu.PF=!(num%2);
	if(op_dest->val<src){
		cpu.CF=1;
	}
	else{
		cpu.CF=0;
	}
	if((flag_src==flag_res&&flag_dest!=flag_src)||(cpu.CF==1&&result==(1<<(DATA_BYTE*8-1)))){
		cpu.OF=1;
	}
	else{
		cpu.OF=0;
	}
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



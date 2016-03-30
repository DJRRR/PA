
#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute(){
	DATA_TYPE flag_dest=MSB(op_dest->val)&1;
	DATA_TYPE flag_src=MSB(op_src->val)&1;
	DATA_TYPE result;
//	if(ops_decoded.opcode==0x83){
//		result=op_dest->val-(DATA_TYPE_S)op_src->val;
//	}
//	else{
	  result = op_dest->val - op_src->val;
//	}
	OPERAND_W(op_dest,result);
	DATA_TYPE flag_res=MSB(result)&1;
//	unsigned int num=0;
//	int i=0;
	cpu.ZF=!result;
	cpu.SF=flag_res;
	if((flag_dest==1&&flag_src==0&&flag_res==0)||(flag_dest==0&&flag_src==1&&flag_res==1)){
		cpu.OF=1;
	}
	else{
		cpu.OF=0;
	}
	if(op_dest->val<op_src->val){
		cpu.CF=1;
	}
	else{
		cpu.CF=0;
	}
/*	for(i=0;i<8;i++){
		if(result&1){
			++num;
		}
		result >>= 1;
	}
	cpu.PF=!(num%2);	*/
	DATA_TYPE res_t=result;
	res_t=res_t&0xff;
	res_t ^= res_t>>4;
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

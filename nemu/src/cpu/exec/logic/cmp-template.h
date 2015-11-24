#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	DATA_TYPE result= op_dest->val-(DATA_TYPE_S)op_src->val;
	unsigned int num=0;
	int i=0;
	cpu.ZF=!result;
	cpu.SF=MSB(result);
	if((MSB(op_dest->val)==1&&MSB(op_src->val)==0&&MSB(result)==0)||(MSB(op_dest->val)==0&&MSB(op_src->val)==1&&MSB(result)==1)){
		cpu.CF=1;
	}
	else{
		cpu.CF=0;
	}
	if(op_dest->val<op_src->val){
		cpu.CF=1;
	}
	else{
		cpu.CF=0;
	}
	for(i=0;i<8;i++){
		if(result&1){
			++num;
		}
		result >>= 1;
	}
	cpu.PF=!(num%2);

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

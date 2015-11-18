#include "cpu/exec/template-start.h"

#define instr cmp

static void do_execute(){
	DATA_TYPE result= op_dest->val - op_src->val;
	if(result==0){
		cpu.ZF=1;
	}
	else{
		cpu.ZF=0;
	}
	if(MSB(result)==1){
		cpu.SF=1;
	}
	else{
		cpu.SF=0;
	}
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

	print_asm_template1();

}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

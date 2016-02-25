#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
	DATA_TYPE result_1=op_dest->val+op_src->val;
	DATA_TYPE tmp1=op_dest->val;
	DATA_TYPE temp1=(~op_src->val+1);
//	unsigned int num=0;
//	int i=0;
	if(tmp1<temp1){
		cpu.CF=1;
		DATA_TYPE result_2=result_1+cpu.CF;
		OPERAND_W(op_dest,result_2);
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

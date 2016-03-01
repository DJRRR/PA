#include "cpu/exec/template-start.h"

#define instr movsx

static void do_execute(){
	DATA_TYPE_S add=op_src->val;
	if(ops_decoded.opcode==0xbe){
		add=(add<<24)>>24;
	}
	else if(ops_decoded.opcode==0xbf){
		add =(add<<16)>>16;
	}
	OPERAND_W(op_dest,add);
	print_asm_template2();
}

make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

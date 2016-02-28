#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	if(ops_decoded.opcode==0xff){
		if(ops_decoded.is_data_size_16){
			cpu.eip=(op_src->val)&0x0000ffff;
		}
		else{
			cpu.eip=op_src->val;
			if(op_src->type==OP_TYPE_REG){
				cpu.eip -= 2;
			}
			else{
				cpu.eip -= 7;
			}
		}
	}
	if(ops_decoded.opcode==0xeb||ops_decoded.opcode==0xe9){
		cpu.eip += op_src->val;
		if(ops_decoded.is_data_size_16){
			cpu.eip=cpu.eip&0x0000ffff;
		}
	}
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)



#include "cpu/exec/template-end.h"



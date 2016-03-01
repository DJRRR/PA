#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	if(ops_decoded.opcode==0xe8){
		if(ops_decoded.is_data_size_16){
			cpu.esp -= 2;
			MEM_W(cpu.esp,(cpu.eip&0xff)+1+DATA_BYTE);
			cpu.eip +=op_src->val&0x0000ffff;
		}
		else{
			cpu.esp -= 4;
			MEM_W(cpu.esp,cpu.eip+1+DATA_BYTE);
			cpu.eip+=op_src->val;
		}
	}
	if(ops_decoded.opcode==0xff){
		if(ops_decoded.is_data_size_16){
			cpu.esp -= 2;
			MEM_W(cpu.esp,(cpu.eip&0xff)+2);
			cpu.eip=((op_src->val)&0xffff)-2;
		}
		else{
			cpu.esp -= 4;
			MEM_W(cpu.esp,cpu.eip+2);
			cpu.eip=op_src->val-2;
		}
	}
	print_asm_template1();

}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"


#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	if(ops_decoded.opcode==0xff){
			cpu.eip=op_src->val;
			if(DATA_BYTE==2){
				cpu.eip = cpu.eip&0x0000ffff;
			}
			if(op_src->type==OP_TYPE_REG){
				cpu.eip -= 2;
			}
			else{
				cpu.eip -= 7;
			}
		
	}
	if(ops_decoded.opcode==0xeb||ops_decoded.opcode==0xe9){
		if(DATA_BYTE==1){
			char t1=op_src->val&0x000000ff;
			cpu.eip += t1;
		}
		else if(DATA_BYTE==2){
			short t2=op_src->val&0x0000ffff;
			cpu.eip=(cpu.eip+t2)&0x0000ffff;
		}
		else{
			cpu.eip += op_src->val;
		}
	}
/*	swaddr_t opcode=instr_fetch(cpu.eip,1);
	DATA_TYPE_S addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	if(opcode==0xeb||opcode==0xe9){
		cpu.eip += addr;
		if(DATA_BYTE==2){
			cpu.eip = cpu.eip&0x0000ffff;
		}
	}
	else{
		cpu.eip=op_src->val;
		if(op_src->type==OP_TYPE_REG){
			cpu.eip -= 2;
		}
		else{
			cpu.eip -= 7;
		}
	}*/
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)



#include "cpu/exec/template-end.h"



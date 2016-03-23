#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	if(ops_decoded.opcode==0xff){
			if(DATA_BYTE==2){
				cpu.eip = cpu.eip&0x0000ffff;
			}
			else{
				cpu.eip=op_src->val;
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
			cpu.eip += t2;
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


make_helper(concat(jmp_ptr_,SUFFIX)){
	printf("%d\n",DATA_BYTE);
	return DATA_BYTE+3;
}
#include "cpu/exec/template-end.h"



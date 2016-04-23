#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute(){
	if(ops_decoded.opcode==0xff){
			if(DATA_BYTE==2){
				cpu.eip = op_src->val&0x0000ffff;
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
	else if(ops_decoded.opcode==0xeb||ops_decoded.opcode==0xe9){
		cpu.eip += (DATA_TYPE_S)op_src->val;
		if(DATA_BYTE==2){
			cpu.eip &= 0x0000ffff;
		}
	}
	else{
		assert(0);
	}
	print_asm_template1();
}

make_instr_helper(i)
make_instr_helper(rm)


make_helper(concat(jmp_ptr_,SUFFIX)){
/*	hwaddr_t addr1=instr_fetch(cpu.eip+1,DATA_BYTE);
	//printf("0x%.8X\n",addr);
	hwaddr_t addr2=instr_fetch(cpu.eip+DATA_BYTE+1,S_ES);
	cpu.eip = addr1;
//	cpu.eip -= DATA+BYTE+3;
	cpu.CS.val=addr2&0x0000ffff;
	cpu.eip -= DATA_BYTE+3;
	if(DATA_BYTE==2){
		cpu.eip &= 0x0000ffff;
	}*/
	cpu.eip=instr_fetch(eip+1,4);
	cpu.eip -= 7;
	cpu.CS.val=instr_fetch(eip+5,2);
	//uint32_t addr3=cpu.CS.index;
	if(DATA_BYTE==2){
		assert(0);
	   	cpu.eip = cpu.eip&0xffff;
	}
	//cpu.DES[1].limit15_0=instr_fetch(cpu.gdtr.base+(addr3*8),2);
	//cpu.DES[1].base15_0=instr_fetch(cpu.gdtr.base+(addr3*8)+2,2);
	//cpu.DES[1].base23_16=instr_fetch(cpu.gdtr.base+(addr3*8)+4,1);
	//cpu.DES[1].limit19_16=instr_fetch(cpu.gdtr.base+(addr3*8)+6,1)&0xf;
	//cpu.DES[1].base31_24=instr_fetch(cpu.gdtr.base+(addr3*8)+7,1);
	print_asm("ljmp $0x%x,0x%x",cpu.CS.val,cpu.eip);
	return 7;
}

#include "cpu/exec/template-end.h"


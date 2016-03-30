#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr call

static void do_execute(){
	int current_sreg=S_SS;
	if(ops_decoded.opcode==0xe8){
		if(ops_decoded.is_data_size_16){
		//	cpu.esp -= 2;
		//	MEM_W(cpu.esp,(cpu.eip&0xff)+1+DATA_BYTE);
		//	cpu.eip +=op_src->val&0xffff;
			cpu.esp-=2;
			MEM_W(cpu.esp,(cpu.eip&0xff)+1+DATA_BYTE);
			cpu.eip += op_src->val&0xffff;
		}
		else{
		/*	if(cpu.eip==0x80480ad){
				printf("here2\n");
				printf("call : 0x%x\n",addr);
				printf("esp : 0x%x\n",cpu.esp);
			}*/
			cpu.esp -= 4;
			MEM_W(cpu.esp,cpu.eip+1+DATA_BYTE);
	/*		if(cpu.eip==0x80480ad){
				printf("here3\n");
			}*/
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
			unsigned int modrm = instr_fetch(cpu.eip+1,1);
			unsigned int mod=modrm>>6;
			if(mod==1 || mod==2){
				MEM_W(cpu.esp,cpu.eip+3);
				cpu.eip = op_src->val-3;
			}
			else{
				MEM_W(cpu.esp,cpu.eip+2);
				cpu.eip=op_src->val-2;
			}
		}
	}
	print_asm_template1();

}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"


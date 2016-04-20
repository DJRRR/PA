#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr call

/*static void do_execute(){
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
			unsigned int modrm = instr_fetch(cpu.eip+1,1);
			unsigned int mod=modrm>>6;
			int buf=0;
			if(mod==1 || mod==2){
				buf=3;
				MEM_W(cpu.esp,cpu.eip+buf);
				cpu.eip = op_src->val;
			}
			else{
				buf=2;
				MEM_W(cpu.esp,cpu.eip+buf);
				cpu.eip=op_src->val;
			}
			if(DATA_BYTE==2){
				cpu.eip &= 0x0000ffff;
			}
			cpu.eip -=buf;
		}
	}
	print_asm_template1();

}*/
static void do_execute(){
	swaddr_t opcode=instr_fetch(cpu.eip,1);
	DATA_TYPE_S addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	int current_sreg=S_SS;
	if(opcode==0xe8){
		cpu.esp -= DATA_BYTE;
		MEM_W(cpu.esp,cpu.eip+DATA_BYTE+1);
		cpu.eip += addr;
		if(DATA_BYTE==2) cpu.eip &=0x0000ffff;
		print_asm("call %x",cpu.eip);
	}
	else{
		uint32_t buf=2;
		if(instr_fetch(cpu.eip+1,1)==0x57) buf=3;
		cpu.esp -=DATA_BYTE;
		MEM_W(cpu.esp,cpu.eip+buf);
		cpu.eip=op_src->val;
		if(DATA_BYTE==2){
			cpu.eip &= 0x0000ffff;
		}
		cpu.eip -= buf;
		print_asm_template1();
	}
}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"


#include "cpu/exec/template-start.h"

#define instr call

static void do_execute(){
	int current_sreg=S_CS;
	if(cpu.eip<=0x8049000){
		printf("cpu.eip:0x%x\n",cpu.eip);
		puts("1");
	}
	DATA_TYPE_S addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	if(ops_decoded.opcode==0xe8){
		if(cpu.eip==0x80480ad){
			printf("here\n");
		}
		if(ops_decoded.is_data_size_16){
		//	cpu.esp -= 2;
		//	MEM_W(cpu.esp,(cpu.eip&0xff)+1+DATA_BYTE);
		//	cpu.eip +=op_src->val&0xffff;
			cpu.esp-=2;
			MEM_W(cpu.esp,cpu.eip+1+DATA_BYTE);
			cpu.eip += addr;
			cpu.eip &= 0xffff;
		}
		else{
	//		printf("here\n");
			cpu.esp -= 4;
			MEM_W(cpu.esp,cpu.eip+1+DATA_BYTE);
			cpu.eip+=addr;
	//		printf("here2\n");
		}
	}
	if(ops_decoded.opcode==0xff){
/*		printf("here\n");
		if(ops_decoded.is_data_size_16){
			cpu.esp -= 2;
			MEM_W(cpu.esp,(cpu.eip&0xff)+2);
			cpu.eip=((op_src->val)&0xffff)-2;
		}
		else{
			cpu.esp -= 4;
			MEM_W(cpu.esp,cpu.eip+2);
			cpu.eip=op_src->val-2;
		}*/
		uint32_t buf=2;
		if(instr_fetch(cpu.eip+1,1)==0x57){
			buf=3;
		}
		MEM_W(cpu.esp,cpu.eip+buf);
		cpu.eip = op_src->val;
		if(DATA_BYTE==2){
			cpu.eip &= 0x0000ffff;
		}
		cpu.eip -= buf;
	}
	print_asm_template1();

}

make_instr_helper(i)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"


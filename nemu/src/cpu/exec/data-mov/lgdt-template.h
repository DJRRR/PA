#include "cpu/exec/template-start.h"

#define instr lgdt

/*static void do_execute(){
//	printf("0x%X\n",instr_fetch(cpu.eip+3,4));
	hwaddr_t addr=instr_fetch(cpu.eip+3,4);
	uint32_t judge=instr_fetch(cpu.eip+2,1);
	printf("!!!!!!!!!%.2X\n",judge);
//	printf("0x%X\n",addr);
	if(ops_decoded.is_data_size_16){
		cpu.gdtr.limit=swaddr_read(addr,2,S_CS)&0x0000ffff;
		cpu.gdtr.base=swaddr_read(addr+2,3,S_CS)&0x00ffffff;
	}
	else{
		cpu.gdtr.limit=swaddr_read(addr,2,S_CS)&0x0000ffff;
		cpu.gdtr.base=swaddr_read(addr+2,4,S_CS);
	}
	print_asm_template1();
}

make_instr_helper(rm);*/

#if DATA_BYTE==2 || DATA_BYTE==4
make_helper(concat(lgdt_i_,SUFFIX)){
	printf("lgdt eip :0x%x\n",cpu.eip);
	uint32_t addr=instr_fetch(cpu.eip+3,4);
	uint32_t base;
	uint8_t opcode=instr_fetch(cpu.eip+2,1);
	uint32_t len;
	if(DATA_BYTE==2){
		base=instr_fetch(instr_fetch(cpu.eip+3,2),4);
		cpu.gdtr.limit=instr_fetch(addr,2)&0x0000ffff;
		cpu.gdtr.base=instr_fetch(addr+2,3)&0x00ffffff;
		len=6;
		print_asm("lgdt 0x%x",base);
	}
	else{
		if(opcode==0x10){
			addr=lnaddr_read(cpu.eax,2);
			cpu.gdtr.limit=instr_fetch(addr,2)&0x0000ffff;
			cpu.gdtr.base=instr_fetch(addr+2,4);
			len=2;
			print_asm("lgdt (%%eax)");
		}
		else{
			base=instr_fetch(cpu.eip+3,4);
			cpu.gdtr.limit=instr_fetch(addr,2)&0x0000ffff;
			cpu.gdtr.base=instr_fetch(addr+2,4);
			len=6;
			print_asm("lgdt 0x%x",base);
		}
	}
	return len;
}

#endif

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-end.h"

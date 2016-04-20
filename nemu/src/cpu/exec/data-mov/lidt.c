#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(lidt_m){
	if(instr_fetch(eip+1,1)==0x18){
	/*	swaddr_t addr=instr_fetch(eip+2,4);
		cpu.idtr.limit=swaddr_read(addr,2,S_SS);
		cpu.idtr.base=swaddr_read(addr+2,4,S_SS);
		print_asm("lidt");
		return 6;*/
		cpu.idtr.limit=swaddr_read(cpu.eax,2,S_SS);
		cpu.idtr.base=swaddr_read(cpu.eax+2,2,S_SS);
		print_asm("lidt (%%eax)");
		return 2;

	}
	else{
		printf("lidt error!\n");
		assert(0);
		cpu.idtr.limit=swaddr_read(cpu.eax,2,S_SS);
		cpu.idtr.base=swaddr_read(cpu.eax+2,2,S_SS);
		print_asm("lidt (%%eax)");
		return 6;
	}
	return 0;
}

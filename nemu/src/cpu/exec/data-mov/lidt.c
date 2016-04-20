#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(lidt_m){
	int judge=instr_fetch(eip+1,1);
	if(judge==0x18){
		cpu.idtr.limit=swaddr_read(cpu.eax,2,S_SS);
		cpu.idtr.base=swaddr_read(cpu.eax+2,4,S_SS);
		print_asm("lidt (%%eax)");
		return 2;
	}
	else{
		printf("lidt error!\n");
		assert(0);
	}
	return 0;
}

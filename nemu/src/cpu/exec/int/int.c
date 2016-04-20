#include "cpu/exec/helper.h"
#include "../../lib-common/x86-inc/mmu.h"
#include <setjmp.h>
extern jmp_buf jbuf;
#define push(x){\
	cpu.esp -= 4;\
	swaddr_write(cpu.esp,4,x,S_SS);\
}

void raise_intr(uint8_t NO){
	push(cpu.EFLAGS);
	push(cpu.CS.val);
	push(cpu.eip);
	GateDesc gd;
	int i;
	for(i=0;i<sizeof(GateDesc);i++){
		*((uint8_t*)(&gd)+i)=lnaddr_read(cpu.idtr.base+NO*sizeof(GateDesc)+i,1);
	}
		cpu.CS.val=gd.segment;
		swaddr_t addr=(gd.offset_31_16<<16)|gd.offset_15_0;
		cpu.eip=addr;
		longjmp(jbuf,1);
}

make_helper(int_i){
	printf("here");
	uint8_t imm=instr_fetch(eip+1,1);
	cpu.eip += 2;
	printf("here");
	print_asm("int\t $0x%02x",imm);
	raise_intr(imm);
	return 2;
}

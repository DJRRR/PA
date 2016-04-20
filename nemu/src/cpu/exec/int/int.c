#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
#include <setjmp.h>
#include "../../lib-common/x86-inc/mmu.h"
extern jmp_buf jbuf;

void push_32(uint32_t temp){
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,temp,S_SS);

}
void push_16(uint16_t temp){
	cpu.esp -= 2;
	swaddr_write(cpu.esp,2,temp,S_SS);
}
void raise_intr(uint8_t NO){
	//protect eflags cs eip
	push_32(cpu.EFLAGS);
	cpu.IF=0;
//	push_32(cpu.eip);
//	push_32(cpu.CS.val);
	push_32(cpu.CS.val);
	push_32(cpu.eip);
	GateDesc tmp;//64 bit
/*	tmp.offset_15_0=lnaddr_read(cpu.idtr.base+NO*8,2)&0xffff;
	tmp.segment=lnaddr_read(cpu.idtr.base+NO*8+2,2)&0xffff;
	tmp.pad0=lnaddr_read(cpu.idtr.base+NO*8+4,1)&0xff;
	tmp.offset_31_16=lnaddr_read(cpu.idtr.base+NO*8+6,2)&0xffff;
//	read GateDesc
	cpu.CS.val=tmp.segment;
	uint32_t res1=tmp.offset_15_0;
	uint32_t res2=tmp.offset_31_16<<16;
	cpu.eip=res1+res2;*/
	int i;
	for(i=0;i<sizeof(GateDesc);i++){
		*((uint8_t*)(&tmp)+i)=lnaddr_read(cpu.idtr.base+NO*sizeof(GateDesc)+i,1);
	}
	cpu.CS.val=tmp.segment;
	swaddr_t addr=(tmp.offset_31_16<<16)+tmp.offset_15_0;
	cpu.eip=addr;

//	printf("here in int.c 0x%x\n",cpu.eip);
	longjmp(jbuf,1);
}

make_helper(int_imm){
	uint8_t judge=instr_fetch(eip+1,1);
//	printf("int.c 0x%x\n",judge);
	cpu.eip += 2;
//	print_asm("int $0x%0x02x",judge);
	raise_intr(judge);
	return 2;
}
	


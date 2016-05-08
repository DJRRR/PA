#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"
#include "device/port-io.h"

#define instr in

make_helper(concat(in_i82a_,SUFFIX)){
	uint32_t judge=instr_fetch(eip,1);
	printf("in-template.h:%d\n",judge);
	uint8_t addr=instr_fetch(eip+1,1);
	REG(R_EAX)=pio_read(addr,DATA_BYTE);
	print_asm("in" str(SUFFIX) " 0x%x,%%%s",addr,REG_NAME(R_EAX));
	return 2;
}


make_helper(concat(in_d2a_,SUFFIX)){
//	uint32_t judge=instr_fetch(cpu.eip,1);
	REG(R_EAX)=pio_read(cpu.gpr[R_DX]._16,DATA_BYTE);
	print_asm("in"str(SUFFIX) " (%%dx), %%%s",REG_NAME(R_EAX));
	return 1;
}
#include "cpu/exec/template-end.h"

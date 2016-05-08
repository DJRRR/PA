#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"
#include "device/port-io.h"

#define instr out

make_helper(concat(out_i82a_,SUFFIX)){
	uint8_t addr=instr_fetch(eip+1,1);
	pio_write(addr,DATA_BYTE,REG(R_EAX));
	print_asm("out" str(SUFFIX) " %%%s, 0x%x",REG_NAME(R_EAX),addr);
	return 2;
}

make_helper(concat(out_d2a_,SUFFIX)){
	pio_write(cpu.gpr[R_DX]._16,DATA_BYTE,REG(R_EAX));
	print_asm("out" str(SUFFIX) " %%%s,(%%dx)",REG_NAME(R_EAX));
	return 1;
}

#include "cpu/exec/template-end.h"

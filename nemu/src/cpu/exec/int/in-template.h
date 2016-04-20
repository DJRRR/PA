#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"
#include "device/port-io.h"

#define instr in

static void do_execute(){
	uint32_t judge=instr_fetch(cpu.eip,1);
	printf("here in2 in.c 0x%x\n",judge);
	print_asm_template2();
}
make_instr_helper(i2a)

make_helper(concat(in_rm2a_,SUFFIX)){
//	uint32_t judge=instr_fetch(cpu.eip,1);
	REG(R_EAX)=pio_read(cpu.gpr[R_DX]._16,DATA_BYTE);
//	print_asm("in"str(SUFFIX) " (%%dx), %%%s",REG_NAME(R_EAX));
	return 1;
}


#include "cpu/exec/template-end.h"

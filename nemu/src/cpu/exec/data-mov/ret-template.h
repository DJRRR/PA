#include "cpu/exec/template-start.h"

#define instr ret
#if DATA_BYTE == 2
make_helper(ret){
	cpu.eip = swaddr_read(cpu.esp , 4 , S_SS) - 1;
	cpu.esp += 4;
	print_asm("ret");
	return 1;
}
static void do_execute() {
	cpu.eip = swaddr_read(cpu.esp , 4 , S_SS);
	cpu.esp += 4;
	cpu.eip -= 3;
	cpu.esp += (DATA_TYPE_S)op_src->val;
	print_asm_template1();
}
make_instr_helper(i)
#endif
#include "cpu/exec/template-end.h"


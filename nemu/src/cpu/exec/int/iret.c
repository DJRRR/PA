#include "cpu/exec/helper.h"
#define DATA_BYTE 4
#include "cpu/exec/template-start.h"

#define pop() swaddr_read(cpu.esp,4,S_SS); cpu.esp += 4;

make_helper(iret){
	cpu.eip=pop();
	cpu.CS.val=pop();
	cpu.EFLAGS=pop();
	print_asm("iret");
	return 0;
}

#include "cpu/exec/template-end.h"


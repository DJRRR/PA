#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(cli){
	cpu.IF=0;//clear interrupt flag
	print_asm("cli");
	return 1;
}


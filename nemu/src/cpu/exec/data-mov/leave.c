#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
make_helper(leave){
	cpu.esp=cpu.ebp;
	if(ops_decoded.is_data_size_16){
		swaddr_write(cpu.ebp,2,(cpu.esp)&0xff);
		cpu.esp += 2;
		print_asm("leave");
	}
	else{
		swaddr_write(cpu.ebp,4,cpu.esp);
		cpu.esp +=4;
		print_asm("leave");
	}
	return 1;
}


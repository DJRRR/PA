#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
make_helper(leave){
	cpu.esp=cpu.ebp;
	if(ops_decoded.is_data_size_16){
		swaddr_write(op_dest->addr,2,(cpu.esp)&0xff);
		cpu.esp += 2;
		print_asm("pop %s","%bp");
	}
	else{
		swaddr_write(op_dest->addr,4,cpu.esp);
		cpu.esp +=4;
		print_asm("pop %s","%ebp");
	}
	return 1;
}


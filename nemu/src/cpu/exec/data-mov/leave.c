#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
make_helper(leave){
	cpu.esp=cpu.ebp;
	if(ops_decoded.is_data_size_16){
		cpu.gpr[5]._16=swaddr_read(cpu.esp,2);
		cpu.esp += 2;
		print_asm("leave");
	}
	else{
		cpu.ebp=swaddr_read(cpu.esp,4)+1;
		cpu.esp +=4;
		print_asm("leave");
	}
	return 1;
}


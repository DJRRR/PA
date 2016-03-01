#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
make_helper(leave){

/*	if(ops_decoded.is_data_size_16){
		cpu.esp=cpu.gpr[5]._16;
		cpu.gpr[5]._16=swaddr_read(cpu.esp,2);
		cpu.esp += 2;
		print_asm("leave");
	}*/
//	else{
		cpu.esp=cpu.ebp;
		cpu.ebp=swaddr_read(cpu.esp,4);
		cpu.esp +=4;
		print_asm("leave");
//	}
	return 1;
}

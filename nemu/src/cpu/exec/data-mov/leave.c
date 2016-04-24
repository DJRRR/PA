#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

#define instr leave

make_helper(leave){
	if(ops_decoded.is_data_size_16){
		cpu.esp=cpu.gpr[5]._16;
		cpu.gpr[5]._16=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
/*		reg_w(R_BP)=swaddr_read(reg_l(R_ESP),2,S_SS);
		reg_l(R_ESP)+=2;*/
		print_asm("leave");
	}
	else{
		cpu.esp=cpu.ebp;
		cpu.ebp=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp +=4;
	/*	reg_l(R_EBP)=swaddr_read(reg_l(R_ESP),4,S_SS);
		reg_l(R_ESP) += 4;*/
		print_asm("leave");
	}
	return 1;
}

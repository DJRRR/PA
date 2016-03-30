#include "cpu/exec/template-start.h"

#define instr ret

make_helper(concat(ret_,SUFFIX)){
		if(ops_decoded.is_data_size_16){
		/*	cpu.eip=swaddr_read(cpu.esp,4,S_CS)-1;
			cpu.esp += 2;
			cpu.eip = cpu.eip&0x0000ffff;*/
			cpu.eip &= 0xffffff00;
			cpu.eip |= swaddr_read(reg_l(R_ESP),2,S_CS);
			reg_l(R_ESP) += 2;
			cpu.eip &= 0x0000ffff;
		}
		else{
			cpu.eip=swaddr_read(reg_l(R_ESP),4,S_CS);
			reg_l(R_ESP)+= 4;
		}
		cpu.esp += op_src->val;
	print_asm("ret");
	return 0;

}

make_helper(concat(ret_imm_,SUFFIX)){//?????
	swaddr_t addr=instr_fetch(cpu.eip+1,DATA_BYTE);
	cpu.eip=swaddr_read(cpu.esp,4,S_CS)-3;
	cpu.esp += addr;
	if(DATA_BYTE==2){
		cpu.eip = cpu.eip &0x0000ffff;
	}
	print_asm_template1();
	return 3;
}

#include "cpu/exec/template-end.h"

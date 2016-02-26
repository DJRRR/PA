#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"


make_helper(cltd){
	if(ops_decoded.is_data_size_16){
		if(cpu.gpr[0]._16<0){
			cpu.gpr[2]._16=0xffff;
		}
		else{
			cpu.gpr[2]._16=0;
		}
		print_asm("cwd");
	}
	else{
		if(cpu.eax<0){
			cpu.edx=0xffffffff;
		}
		else{
			cpu.edx=0;
		}
		print_asm("cdq");
	}
	return 1;
}


#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"
//#include "cpu/exec/template-start.h"
make_helper(ret){
	if(ops_decoded.opcode==0xc3){
		if(ops_decoded.is_data_size_16){
			cpu.eip=swaddr_read(cpu.esp,2);//unchecked
			cpu.esp += 2;
			cpu.eip = cpu.eip & 0xffff;
			print_asm("ret");
		}
		else{
			cpu.eip=swaddr_read(cpu.esp,4);
			cpu.esp += 4;
			print_asm("ret");
		}
	}
	return 1;
}

//#include "cpu/exec/template-end.h"	

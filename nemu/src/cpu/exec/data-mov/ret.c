#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "ret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ret-template.h"
#undef DATA_BYTE

make_helper_v(ret)
make_helper_v(ret_imm)

/*make_helper(ret){
	uint32_t imm16=instr_fetch(cpu.eip+1,2);
	if(ops_decoded.is_data_size_16){
		cpu.eip=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
		cpu.eip &= 0x0000ffff;
	}
	else{
		cpu.eip=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
	}
	if(ops_decoded.opcode==0xc2){
		cpu.esp += imm16;
		print_asm("ret $0x%x",imm16);
	}
	else{
		print_asm("ret");
	}
	cpu.eip --;
	return 1;
}*/


#include "cpu/exec/template-start.h"

#define instr je

static void do_execute(){
	if(cpu.ZF==1){
		int temp_addr=instr_fetch(cpu.eip+1,DATA_BYTE);
		int shift=32-(DATA_BYTE<<3);
		temp_addr=(temp_addr<<shift)>>shift;
		cpu.eip += temp_addr;
		if(ops_decoded.is_data_size_16){
			cpu.eip = cpu.eip&0x0000ffff;
		}
	}
	print_asm_template1();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"

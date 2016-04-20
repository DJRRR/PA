#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

/*void push_32(uint32_t val){
	cpu.esp -= 4;
	swaddr_write(cpu.esp,4,val,S_SS);
}
void push_16(uint16_t val){
	cpu.esp -= 2;
	swaddr_write(cpu.esp,2,val,S_SS);
}*/
//defined in int.c
extern void push_32(uint32_t temp);
extern void push_16(uint16_t temp);
make_helper(pusha){
	if(ops_decoded.is_data_size_16){
		assert(0);
		push_16(cpu.gpr[0]._16);
		push_16(cpu.gpr[1]._16);
		push_16(cpu.gpr[2]._16);
		push_16(cpu.gpr[3]._16);
		push_16(cpu.gpr[4]._16);
		push_16(cpu.gpr[5]._16);
		push_16(cpu.gpr[6]._16);
		push_16(cpu.gpr[7]._16);
		print_asm("pusha");
	}
	else{
		swaddr_t temp2=cpu.esp;
		push_32(cpu.eax);
		push_32(cpu.ecx);
		push_32(cpu.edx);
		push_32(cpu.ebx);
		push_32(temp2);
		push_32(cpu.ebp);
		push_32(cpu.esi);
		push_32(cpu.edi);
		print_asm("pushad");
	}
	return 1;
}




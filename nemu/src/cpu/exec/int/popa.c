#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(popa){
	if(ops_decoded.is_data_size_16){
	//	printf("here popa 16\n");
		cpu.gpr[7]._16=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
		cpu.gpr[6]._16=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
		cpu.gpr[5]._16=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
		cpu.esp += 2;//throwaway esp
		cpu.gpr[3]._16=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
		cpu.gpr[2]._16=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
		cpu.gpr[1]._16=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
		cpu.gpr[0]._16=swaddr_read(cpu.esp,2,S_SS);
		cpu.esp += 2;
		print_asm("popa");
	}
	else{
		cpu.edi=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
		cpu.esi=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
		cpu.ebp=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
		cpu.esp += 4;//throwaway esp
		cpu.ebx=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
		cpu.edx=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
		cpu.ecx=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
		cpu.eax=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
		print_asm("popad");
	}
	return 1;
}

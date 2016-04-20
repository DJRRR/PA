#include "cpu/exec/helper.h"
#define DATA_BYTE 4
//#include "cpu/decode/modrm.h"
#include "cpu/exec/template-start.h"
make_helper(iret){
//	printf("here  int iret.c\n");
	if(ops_decoded.is_data_size_16){
		printf("iret 16bit\n");
		assert(0);
	}
	else{
	//	asm volatile("int3");
   // 	printf("here int iret.c opcode:0x%x\n",instr_fetch(cpu.eip,1));
		cpu.eip=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
	//	printf("here int iret.c cpu.eip\n");
//		cpu.CS.val=swaddr_read(cpu.esp,2,S_SS);
//		cpu.esp += 2;
		cpu.CS.val=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
	//	printf("here int iret.c cpu.CS\n");
		cpu.EFLAGS=swaddr_read(cpu.esp,4,S_SS);
		cpu.esp += 4;
	//	printf("here in iret.c eflags\n");
		print_asm("iret");
	//	printf("next opcode : 0x%x\n",instr_fetch(cpu.eip+1,1));
	}
	//return 1;
	return 0;
}

#include "cpu/exec/template-start.h"

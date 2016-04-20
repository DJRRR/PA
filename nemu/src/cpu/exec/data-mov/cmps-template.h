#include "cpu/exec/template-start.h"

make_helper(concat(cmps_,SUFFIX)){
	uint32_t src_index=cpu.esi;
	uint32_t dest_index=cpu.edi;
	uint32_t incdec=0;
	uint32_t result=swaddr_read(src_index,DATA_BYTE,S_ES)-swaddr_read(dest_index,DATA_BYTE,S_DS);
	if(cpu.DF==0){
		incdec=DATA_BYTE;
	}
	else{
		incdec=-DATA_BYTE;
	}
	cpu.SF=MSB(result);
	cpu.CF=(result>swaddr_read(dest_index,DATA_BYTE,S_DS))?1:0;
	cpu.ZF=!result;
	DATA_TYPE p=result;
	p &= 0xff;
	p ^=p>>4;
	p^=p>>2;
	p^=p>>1;
	cpu.PF=!p;
	if(MSB(swaddr_read(src_index,DATA_BYTE,S_ES))!=MSB(swaddr_read(dest_index,DATA_BYTE,S_DS))&&MSB(result)==MSB(swaddr_read(src_index,DATA_BYTE,S_ES))){
		cpu.OF=1;
	}
	else{
		cpu.OF=0;
	}
	cpu.esi=src_index+incdec;
	cpu.edi=dest_index+incdec;
	print_asm("cmps %%edi,%%esi\n");
	return 1;
}

#include "cpu/exec/template-end.h"




#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat(cmps_m_,SUFFIX)){
	DATA_TYPE src=swaddr_read(cpu.esi,DATA_BYTE,1);
	DATA_TYPE dest=swaddr_read(cpu.edi,DATA_BYTE,2);
	DATA_TYPE result=dest-src;
	DATA_TYPE flag_dest=MSB(dest)&1;
	DATA_TYPE flag_src=MSB(src)&1;
	DATA_TYPE flag_res=MSB(result)&1;
	unsigned int num=0;
	int i=0;
	cpu.ZF=!result;
	cpu.SF=flag_res;
	if((flag_dest==1&&flag_src==0&&flag_res==0)||(flag_dest==0&&flag_src==1&&flag_res==1)){
		cpu.OF=1;
	}
	else{
		cpu.OF=0;
	}
	if(dest<src){
		cpu.CF=1;
	}
	else{
		cpu.CF=0;
	}
	for(i=0;i<8;i++){
		if(result&1){
			num++;
		}
		result >>= 1;
	}
	cpu.PF=!(num%2);
	if(DATA_BYTE==1){
		if(cpu.DF==0){
			cpu.edi++;
			cpu.esi++;
		}
		else{
			cpu.edi--;
			cpu.esi--;
		}
	}
	else if(DATA_BYTE==2){
		if(cpu.DF==0){
			cpu.edi += 2;
			cpu.esi += 2;
		}
		else{
			cpu.edi -= 2;
			cpu.esi -= 2;
		}
	}
	else{
		if(cpu.DF==0){
			cpu.edi += 4;
			cpu.esi += 4;
		}
		else{
			cpu.edi -= 4;
			cpu.esi -= 4;
		}
	}
	print_asm_template2();
	return DATA_BYTE+1;
}

#include "cpu/exec/template-end.h"



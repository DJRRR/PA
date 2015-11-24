
#include "cpu/exec/template-start.h"

#define instr sub

static void do_execute(){
	DATA_TYPE flag_dest=(op_dest->val>>(DATA_BYTE*8-1))&1;
	DATA_TYPE flag_src=(op_src->val>>(DATA_BYTE*8-1))&1;
	DATA_TYPE result = op_dest->val - op_src->val;
	//printf("test:%u",result);
	OPERAND_W(op_dest,result);
	DATA_TYPE flag_res=(result>>(DATA_BYTE*8-1))&1;
	unsigned int num=0;
	int i=0;
	for(i=0;i<8;i++){
		if(result&1){
			num++;
		}
		result >>= 1;
	}
	cpu.PF=!(num%2);
/*	if(num%2==0){
		cpu.PF=1;
	}
	else{
		cpu.PF=0;
	}*/
   /* if(result==0){
		cpu.ZF=1;
	}
	else{
		cpu.ZF=0;
	}*/
	cpu.ZF=!result;
	cpu.SF=flag_res;
/*	if(flag_res==1){
		cpu.SF=1;
	}
	else{
		cpu.SF=0;
	}*/
	if((flag_dest==1&&flag_src==0&&flag_res==0)||(flag_dest==0&&flag_src==1&&flag_res==1)){
		cpu.OF=1;
	}
	else{
		cpu.OF=0;
	}
	if(op_dest->val<op_src->val){
		cpu.CF=1;
	}
	else{
     	cpu.CF=0;
	}	

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE==2 || DATA_BYTE==4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

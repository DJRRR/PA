#include "cpu/exec/template-start.h"

#define instr adc

static void do_execute(){
/*	DATA_TYPE result_1=op_dest->val+op_src->val;//wrong hit bad trap
	DATA_TYPE tmp1=op_dest->val;
	DATA_TYPE temp1=(~op_src->val+1);
	unsigned int num=0;
	int i=0;
	if(tmp1<temp1){
		cpu.CF=1;
		DATA_TYPE result_2=result_1+cpu.CF;
		DATA_TYPE tmp2=result_1;
		DATA_TYPE temp2=(~cpu.CF+1);
		DATA_TYPE flag_dest_1=MSB(result_1)&1;
		DATA_TYPE flag_src_1=MSB(cpu.CF)&1;
		DATA_TYPE flag_res_1=MSB(result_2)&1;
		OPERAND_W(op_dest,result_2);
		if(tmp2<temp2){
			cpu.CF=1;
		}
		else{
			cpu.CF=0;
		}
		cpu.ZF=!result_2;
		cpu.SF=flag_res_1;
		if((flag_dest_1==1&&flag_src_1==1&&flag_res_1==0)||(flag_dest_1==0&&flag_src_1==0&&flag_res_1==1)){
			cpu.OF=1;
		}
		else{
			cpu.OF=0;
		}
		for(i=0;i<8;i++){
			if(result_2&1){
				num++;
			}
			result_2 >>= 1;
		}
		cpu.PF=!(num%2);
	}

	else{
		DATA_TYPE flag_dest_2=MSB(op_dest->val)&1;
		DATA_TYPE flag_src_2=MSB(op_src->val)&1;
		DATA_TYPE flag_res_2=MSB(result_1)&1;
		OPERAND_W(op_dest,result_1);
		if(tmp1<temp1){
			cpu.CF=1;
		}
		else{
			cpu.CF=0;
		}
		cpu.ZF=!result_1;
		cpu.SF=flag_res_2;
		if((flag_dest_2==1&&flag_src_2==1&&flag_res_2==0)||(flag_dest_2==0&&flag_src_2==0&&flag_res_2==1)){
			cpu.OF=1;
		}
		else{
			cpu.OF=0;
		}
		for(i=0;i<8;i++){
			if(result_1&1){
				num++;
			}
			result_1 >>= 1;
		}
		cpu.PF=!(num%2);
	}	*/
	DATA_TYPE result=op_dest->val+op_src->val+cpu.CF;
	OPERAND_W(op_dest,result);
	DATA_TYPE temp=result;
	DATA_TYPE flag_dest=MSB(op_dest->val)&1;
	DATA_TYPE flag_src=MSB(op_src->val)&1;
	DATA_TYPE flag_res=MSB(result)&1;
	unsigned int num=0;
	int i=0;
	cpu.SF=flag_res;
	cpu.ZF=!result;
	for(i=0;i<8;i++){
		if(temp&1){
			num++;
		}
		temp >>= 1;
	}
	cpu.PF=!(num%2);
	if(result<op_src->val||result<op_dest->val){
		cpu.CF=1;
	}
	else{
		cpu.CF=0;
	}
	if((flag_dest==flag_src&&flag_src!=flag_res)||(cpu.CF==1&&result==(1<<(DATA_BYTE*8-1)))){
		cpu.OF=1;
	}
	else{
		cpu.OF=0;
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

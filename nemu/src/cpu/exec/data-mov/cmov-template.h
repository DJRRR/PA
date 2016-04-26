#include "cpu/exec/template-start.h"

#define instr cmov

#if DATA_BYTE==2||DATA_BYTE==4

static void do_execute(){
//	printf("test:%u\n",ops_decoded.opcode);
	DATA_TYPE opcode=instr_fetch(cpu.eip+1,1);
	switch(opcode&0xff){
	case 0x40:
		if(cpu.OF==1){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x41:
		if(cpu.OF==0){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x42:
		if(cpu.CF==1){
			OPERAND_W(op_dest,op_src->val);
		}
		break;	
	case 0x43:
		if(cpu.CF==0){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x44:
		if(cpu.ZF==1){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x45:
		if(cpu.ZF==0){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x46:
		if(cpu.CF==1||cpu.ZF==1){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x47:
		if(cpu.CF==0&&cpu.ZF==0){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x48:
		if(cpu.SF==1){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x49:
		if(cpu.SF==0){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x4a:
		if(cpu.PF==1){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x4b:
		if(cpu.PF==0){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x4c:
		if(cpu.OF!=cpu.SF){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x4d:
		if(cpu.OF==cpu.SF){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x4e:
		if(cpu.ZF==1||(cpu.SF!=cpu.OF)){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	case 0x4f:
		if(cpu.ZF==0&&(cpu.SF==cpu.OF)){
			OPERAND_W(op_dest,op_src->val);
		}
		break;
	default:
		break;
	}
	print_asm_template2();
}

make_instr_helper(rm2r)

#endif
#include "cpu/exec/template-end.h"

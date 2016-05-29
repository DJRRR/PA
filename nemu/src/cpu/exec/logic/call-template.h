#include "cpu/exec/template-start.h"


#define instr call

static void do_execute() {
//	uint32_t ceip,cceip ;
	cpu.esp -= 4;
	op_dest->type = OP_TYPE_MEM;
	op_dest->size = 4;
	op_dest->addr = cpu.esp;
#if DATA_BYTE == 2
	uint32_t instrlen = 4;
#endif
#if DATA_BYTE == 4
	uint32_t instrlen = 5;
#endif 
	OPERAND_W(op_dest, cpu.eip + instrlen);
	cpu.eip += (DATA_TYPE_S)(op_src->val);
//	cceip = ceip = cpu.eip + instrlen;
//	print_asm_template4();
}
#if DATA_BYTE == 2
make_helper(call_rm_w){
	int leng = decode_rm_w(eip + 1) + 1;
//	swaddr_t ceip; 
//	ceip = op_src->val;
	cpu.esp -= 4;
	op_dest->type = OP_TYPE_MEM;
	op_dest->size = 4;
	op_dest->addr = cpu.esp;
	OPERAND_W(op_dest, cpu.eip + leng + 1);
	cpu.eip = op_src->val - leng - 1;
//	print_asm_template6();
	return leng;
}
#endif
#if DATA_BYTE == 4
make_helper(call_rm_l){
	int leng = decode_rm_l(eip + 1) + 1;
//	swaddr_t ceip; 
//	ceip = op_src->val;
	cpu.esp -= 4;
	op_dest->type = OP_TYPE_MEM;
	op_dest->size = 4;
	op_dest->addr = cpu.esp;
	OPERAND_W(op_dest, cpu.eip + leng);
	cpu.eip = op_src->val - leng;
//	print_asm_template6();
	return leng;
}
#endif

make_instr_helper(i)

#include "cpu/exec/template-end.h"


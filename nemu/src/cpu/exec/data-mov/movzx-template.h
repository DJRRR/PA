#include "cpu/exec/template-start.h"

#define instr movzb

/*static void do_execute(){
	DATA_TYPE res=op_src->val&0xffff;
	reg_l(op_dest->reg)=res;

	print_asm_template2();

}*/
static void do_execute(){
	DATA_TYPE result=0xff&op_src->val;
	OPERAND_W(op_dest,result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#define instr movzw
static void do_execute(){
	DATA_TYPE result=0xffff&op_src->val;
	OPERAND_W(op_dest,result);
	print_asm_template2();
}
make_instr_helper(rm2r)
#undef instr

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr smsw

static void do_execute(){
	printf("here!\n");
	OPERAND_W(op_src,cpu.cr0.val);
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

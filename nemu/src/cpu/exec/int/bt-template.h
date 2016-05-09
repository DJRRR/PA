#include "cpu/exec/template-start.h"
#include "cpu/decode/modrm.h"

#define instr bt

static void do_execute(){
	cpu.CF=(op_dest->val>>op_src->val)&1;
	print_asm_template2();
}

make_instr_helper(rm2r)


#include "cpu/exec/template-end.h"

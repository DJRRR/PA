#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
	printf("0x%X\n",op_src->val);
	if(ops_decoded.is_data_size_16){


	}
	else{
	}
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

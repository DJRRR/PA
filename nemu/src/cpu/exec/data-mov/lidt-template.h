#include "cpu/exec/template-start.h"

#define instr lidt

static void do_execute(){
	if(ops_decoded.is_data_size_16){
	}
	else{
	}
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
/*	if(ops_decoded.is_data_size_16){
        OPERAND_W(op_dest,(cpu.esp)&0xff);
		cpu.esp += 2;
	}
	else{
		OPERAND_W(op_dest,cpu.esp);
		cpu.esp += 4;
	}*/
	OPERAND_W(op_src,MEM_R(cpu.esp));//unchecked
	cpu.esp += DATA_BYTE;
    print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"

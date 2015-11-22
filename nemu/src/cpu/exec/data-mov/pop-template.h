#include "cpu/exec/template-start.h"

#define instr pop

static void do_execute(){
	if(ops_decoded.is_data_size_16){
        MEM_W(op_dest->addr,(cpu.esp&0xff));
	  //  op_dest->val=(cpu.esp&0xff);
		cpu.esp += 2;
	}
	else{
		MEM_W(op_dest->addr,cpu.esp);
	 //   op_dest->val=cpu.esp;
		cpu.esp += 4;
	}
    print_asm_template1();
}

make_instr_helper(r)
make_instr_helper(rm)

#include "cpu/exec/template-end.h"

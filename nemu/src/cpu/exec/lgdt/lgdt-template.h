#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute(){
	printf("0x%.2X\n",ops_decoded.opcode);
	

}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"

#include "cpu/exec/template-start.h"
#define instr sbb

static void do_execute(){
	uint64_t cfcf = (uint64_t)op_dest->val - op_src->val - cpu.CF;
	DATA_TYPE result = op_dest->val - op_src->val;
	OPERAND_W(op_dest, result-cpu.CF);
	uint32_t CF,OF;
	CF = ((cfcf>>(DATA_BYTE<<3))&1);
	OF = (MSB(op_dest->val) ^ MSB(result)) && (MSB(op_dest->val) ^ MSB(op_src->val));
//	cal_eflags();
//	// 未实现eflags的功能
	cpu.CF=CF;
	cpu.OF=OF;
	cpu.ZF=!result;
	cpu.SF=MSB(result);
	uint32_t r=result;
	r ^= r>>4;
	r ^= r>>2;
	r ^= r>>1;
	cpu.PF=!(r&1);
	print_asm_template2();
}
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"

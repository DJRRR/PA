#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(nop) {
	print_asm("nop");
	return 1;
}

make_helper(int3) {
	void do_int3();
	do_int3();
	print_asm("int3");

	return 1;
}
make_helper(cwtl){
	int32_t result=(int16_t)reg_w(R_AX);
	reg_l(R_EAX)=result;
	print_asm("cwtl");
	return 1;
}
make_helper(lea) {
	ModR_M m;
	m.val = instr_fetch(eip + 1, 1);
	int len = load_addr(eip + 1, &m, op_src);
	reg_l(m.reg) = op_src->addr;

	print_asm("leal %s,%%%s", op_src->str, regsl[m.reg]);
	return 1 + len;
}
make_helper(hlt){
	while(!(cpu.INTR&&cpu.IF));
	print_asm("hlt");
	return 1;
}

make_helper(sti){
	cpu.IF=1;
	print_asm("sti");
	return 1;
}

#include "cpu/exec/helper.h"

extern uint32_t pio_read(ioaddr_t , size_t);
extern void pio_write(ioaddr_t , size_t ,uint32_t);

make_helper(in_i_b) {
	reg_b(R_AL)=pio_read(instr_fetch(eip+1,1),1);
	print_asm("in");
	return 2;
}

make_helper(in_i_w) {
	reg_w(R_AX)=pio_read(instr_fetch(eip+1,1),2);
	print_asm("in");
	return 2;
}

make_helper(in_i_l) {
	reg_l(R_EAX)=pio_read(instr_fetch(eip+1,1),4);
	print_asm("in");
	return 2;
}

make_helper(in_d_b) {
	reg_b(R_AL)=pio_read(reg_w(R_DX),1);
	print_asm("in");
	return 1;
}

make_helper(in_d_w) {
	reg_w(R_AX)=pio_read(reg_w(R_DX),2);
	print_asm("in");
	return 1;
}

make_helper(in_d_l) {
	reg_l(R_EAX)=pio_read(reg_w(R_DX),4);
	print_asm("in");
	return 1;
}

make_helper(out_i_b) {
	pio_write(instr_fetch(eip+1,1),1,reg_b(R_AL));
	print_asm("out");
	return 2;
}

make_helper(out_i_w) {
	pio_write(instr_fetch(eip+1,1),2,reg_w(R_AX));
	print_asm("out");
	return 2;
}

make_helper(out_i_l) {
	pio_write(instr_fetch(eip+1,1),4,reg_l(R_EAX));
	print_asm("out");
	return 2;
}

make_helper(out_d_b) {
	pio_write(reg_w(R_DX),1,reg_b(R_AL));
	print_asm("out");
	return 1;
}
make_helper(out_d_w) {
	pio_write(reg_w(R_DX),2,reg_w(R_AX));
	print_asm("out");
	return 1;
}
make_helper(out_d_l) {
	pio_write(reg_w(R_DX),4,reg_l(R_EAX));
	print_asm("out");
	return 1;
}
make_helper_v(in_d)
make_helper_v(in_i)
make_helper_v(out_d)
make_helper_v(out_i)

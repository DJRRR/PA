#include "cpu/exec/helper.h"

uint32_t pio_read(ioaddr_t,size_t);
uint32_t pio_write(ioaddr_t,size_t,uint32_t);

make_helper(in_i_b){
	int len=decode_i_b(eip+1)+1;
	reg_b(R_AL)=pio_read(op_src->val,1);
	print_asm("in" "0x%x, al",op_src->val);
	return len;
}
make_helper(in_i_w){
	int len=decode_i_b(eip+1)+1;
	reg_w(R_AX)=pio_read(op_src->val,2);
	print_asm("in" "0x%x, ax",op_src->val);
	return len;
}
make_helper(in_i_l){
	int len=decode_i_b(eip+1)+1;
	reg_l(R_EAX)=pio_read(op_src->val,4);
	print_asm("in" "0x%x, eax",op_src->val);
	return len;
}

make_helper(in_b){
	reg_b(R_AL)=pio_read(reg_w(R_DX),1);
	print_asm("in" " (dx), al");
	return 1;
}
make_helper(in_w){
	reg_w(R_AX)=pio_read(reg_w(R_DX),2);
	print_asm("in" " (dx), ax");
	return 1;
}
make_helper(in_l){
	reg_l(R_EAX)=pio_read(reg_w(R_DX),4);
	print_asm("in" " (dx),eax");
	return 1;
	
}

make_helper_v(in)
make_helper_v(in_i)

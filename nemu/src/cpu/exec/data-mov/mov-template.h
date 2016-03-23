#include "cpu/exec/template-start.h"

#define instr mov

static void do_execute() {
	OPERAND_W(op_dest, op_src->val);
	print_asm_template2();
}

make_instr_helper(i2r)
make_instr_helper(i2rm)
make_instr_helper(r2rm)
make_instr_helper(rm2r)
//make_instr_helper(cr2r)
//make_instr_helper(r2cr)

make_helper(concat(mov_cr2r_,SUFFIX)){
	uint32_t judge=instr_fetch(cpu.eip+1,1);
//	printf("0x%.2X\n",judge);
	if(judge==0xc0){//cr0
		REG(op_src->reg)=cpu.cr0.val;
		puts("1");
		print_asm("mov cr0 ,  %%%s",REG_NAME(op_src->reg));

	}
	else if(judge==0xd8){//cr3
		REG(op_src->reg)=cpu.cr3.val;
		print_asm("mov cr3 , %%%s",REG_NAME(op_src->reg));
	}
	printf("%%%s\n",REG_NAME(op_src->reg));
	return 2;

}
make_helper(concat(mov_r2cr_,SUFFIX)){
	uint32_t judge=instr_fetch(cpu.eip+1,1);
	if(judge==0xc0){//cr0
	//	cpu.cr0.val=REG(op_sr
	}
	else if(judge==0xd8){//cr3
	}
	return 2;
}
make_helper(concat(mov_a2moffs_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"

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
make_instr_helper(rm)
//make_instr_helper(cr2r)
//make_instr_helper(r2cr)

make_helper(concat(mov_cr2r_,SUFFIX)){
	int len=decode_rm_l(cpu.eip+2);
	printf("cr2r len:%d\n",len);
	uint32_t judge=instr_fetch(cpu.eip+2,1);
	if(judge==0xc0){//cr0
		REG(op_src->reg)=cpu.cr0.val;
		print_asm("mov cr0,%%%s",REG_NAME(op_src->reg));

	}
	else if(judge==0xd8){//cr3
		REG(op_src->reg)=cpu.cr3.val;
		print_asm("mov cr3,%%%s",REG_NAME(op_src->reg));
	}
	return 1+len;

}
make_helper(concat(mov_r2cr_,SUFFIX)){
	int len=decode_rm_l(cpu.eip+2);
	printf("r2cr len : %d\n",len);
	uint32_t judge=instr_fetch(cpu.eip+2,1);
//	if(judge==0xc0){//cr0
//		cpu.cr0.val=REG(R_EAX);
//		print_asm("mov %%%s,cr0",REG_NAME(op_src->reg));
//	}
   if(judge==0xd8){//cr3
		cpu.cr3.val=REG(op_src->reg);
		init_page_L1();
		print_asm("mov %%%s,cr3",REG_NAME(op_src->reg));
	}
   else{
	   cpu.cr0.val=REG(op_src->reg);
	   printf("test. mov r2cr\n");
	   print_asm("mov %%%s,cr0",REG_NAME(op_src->reg));
   }
	return 1+len;
}
make_helper(concat(mov_r2seg_,SUFFIX)){//just read limit and base
	uint32_t judge=instr_fetch(cpu.eip+2,1);
//	printf("0x%.2X\n",judge);
	if(judge==0xd8){//ds
		cpu.DS.val=op_src->val;
//		printf("1111\n");
		uint32_t addr1=cpu.DS.index;
		cpu.DES[1].limit15_0=lnaddr_read(cpu.gdtr.base+(addr1<<3),2);//16
		cpu.DES[1].base15_0=lnaddr_read(cpu.gdtr.base+(addr1<<3)+2,2);//16
		cpu.DES[1].base23_16=lnaddr_read(cpu.gdtr.base+(addr1<<3)+4,1);//8
		cpu.DES[1].limit19_16=lnaddr_read(cpu.gdtr.base+(addr1<<3)+6,1)&0xf;//4
		cpu.DES[1].base31_24=lnaddr_read(cpu.gdtr.base+(addr1<<3)+7,1);//8;
		print_asm("mov %%%s,ds",REG_NAME(op_src->reg));

	}
	else if(judge==0xc0){//es
	//	cpu.ES.val=REG(op_src->reg);
		cpu.ES.val=op_src->val;
//		printf("2222\n");
		uint32_t addr2=cpu.ES.index;
		cpu.DES[2].limit15_0=lnaddr_read(cpu.gdtr.base+(addr2<<3),2);
		cpu.DES[2].base15_0=lnaddr_read(cpu.gdtr.base+(addr2<<3)+2,2);
		cpu.DES[2].base23_16=lnaddr_read(cpu.gdtr.base+(addr2<<3)+4,1);
		cpu.DES[2].limit19_16=lnaddr_read(cpu.gdtr.base+(addr2<<3)+6,1)&0xf;
		cpu.DES[2].base31_24=lnaddr_read(cpu.gdtr.base+(addr2<<3)+7,1);
		print_asm("mov %%%s,es",REG_NAME(op_src->reg));

	}
	else if(judge==0xd0){//ss
	//	cpu.SS.val=REG(op_src->reg);
		cpu.SS.val=op_src->val;
	//	printf("3333\n");
		uint32_t addr3=cpu.SS.index;
		cpu.DES[3].limit15_0=lnaddr_read(cpu.gdtr.base+(addr3<<3),2);
		cpu.DES[3].base15_0=lnaddr_read(cpu.gdtr.base+(addr3<<3)+2,2);
		cpu.DES[3].base23_16=lnaddr_read(cpu.gdtr.base+(addr3<<3)+4,1);
		cpu.DES[3].limit19_16=lnaddr_read(cpu.gdtr.base+(addr3<<3)+6,1)&0xf;
		cpu.DES[3].base31_24=lnaddr_read(cpu.gdtr.base+(addr3<<3)+7,1);
		print_asm("mov %%%s,ss",REG_NAME(op_src->reg));

	}
	else{//cs
	//	cpu.CS.val=REG(op_src->reg);
		cpu.CS.val=op_src->val;
	//	printf("4444\n");
		uint32_t addr4=cpu.CS.index;
		cpu.DES[0].limit15_0=lnaddr_read(cpu.gdtr.base+(addr4<<3),2);
		cpu.DES[0].base15_0=lnaddr_read(cpu.gdtr.base+(addr4<<3)+2,2);
		cpu.DES[0].base23_16=lnaddr_read(cpu.gdtr.base+(addr4<<3)+4,1);
		cpu.DES[0].limit19_16=lnaddr_read(cpu.gdtr.base+(addr4<<3)+6,1)&0xf;
		cpu.DES[0].base31_24=lnaddr_read(cpu.gdtr.base+(addr4<<3)+7,1);
		print_asm("mov %%%s,cs",REG_NAME(op_src->reg));

	}
	return 2;

}
make_helper(concat(mov_a2moffs_, SUFFIX)) {
	int current_sreg=1;
	swaddr_t addr = instr_fetch(eip + 1, 4);
	MEM_W(addr, REG(R_EAX));

	print_asm("mov" str(SUFFIX) " %%%s,0x%x", REG_NAME(R_EAX), addr);
	return 5;
}

make_helper(concat(mov_moffs2a_, SUFFIX)) {
	int current_sreg=1;
	swaddr_t addr = instr_fetch(eip + 1, 4);
	REG(R_EAX) = MEM_R(addr);

	print_asm("mov" str(SUFFIX) " 0x%x,%%%s", addr, REG_NAME(R_EAX));
	return 5;
}

#include "cpu/exec/template-end.h"

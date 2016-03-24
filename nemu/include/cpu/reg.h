#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../lib-common/x86-inc/cpu.h"
#include "../../lib-common/x86-inc/mmu.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

typedef struct {
	union{
	union {
		uint32_t _32;
		uint8_t _8[2];
		uint16_t _16;
	}gpr[8];
	/* Do NOT change the order of the GPRs' definitions. */
	struct{
	uint32_t eax, ecx, edx, ebx, esp, ebp, esi, edi;
	};
	};
	union{
		uint32_t EFLAGS;
		struct{
			unsigned int CF:1;
			unsigned int tmp_1:1;
			unsigned int PF:1;
			unsigned int tmp_2:3;
			unsigned int ZF:1;
			unsigned int SF:1;
			unsigned int tmp_3:1;
			unsigned int IF:1;
			unsigned int DF:1;
			unsigned int OF:1;
			unsigned int tmp_4:20;
		};
	};
	CR0 cr0;//defined in x-86/inc/cpu.h
	CR3 cr3;
	union {
		struct{
			unsigned int rpl:2;
			unsigned int tl:1;
			unsigned int index:13;
		};
		uint16_t val;
	}CS,DS,ES,SS;
	struct{
		union{
			struct{
				unsigned int limit15_0:16;
				unsigned int base15_0:16;
			};
			uint32_t val1;
		};
		union{
			struct{
				unsigned int base23_16:8;
				unsigned int A:1;
				unsigned int type:3;
				unsigned int S_type:1;
				unsigned int DPL:2;//tequanji
				unsigned int S_present:1;
				unsigned int limit19_16:4;
				unsigned int AVL:1;
				unsigned int O:1;
				unsigned int X:1;
				unsigned int G:1;
				unsigned int base31_24:8;
			};
			uint32_t val2;
		};
	}CS_DES,DS_DES,ES_DES,SS_DES;

	struct {
		uint16_t bound;
		uint32_t base;
	}gdtr;
	struct {
		uint16_t bound;
		uint32_t base;
	}idtr;

	swaddr_t eip;
} CPU_state;

extern CPU_state cpu;

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}

#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define reg_b(index) (cpu.gpr[check_reg_index(index) & 0x3]._8[index >> 2])

extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif

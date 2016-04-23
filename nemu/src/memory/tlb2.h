#include <common.h>

typedef struct{
	uint8_t valid;
	uint32_t tag;
	uint32_t page_data;
}TLB;
TLB tlb[64];

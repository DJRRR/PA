#include "common.h"
#include "nemu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t read_cache(hwaddr_t,size_t);
void write_cache(hwaddr_t,size_t,uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
//	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	return read_cache(addr,len) & (~0u >> ((4-len)<<3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
//	dram_write(addr, len, data);
	write_cache(addr,len,data);
}
hwaddr_t page_translate(lnaddr_t addr){
	hwaddr_t res;
	res=addr;
	return res;
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	//return hwaddr_read(addr, len);
#ifdef DEBUG
	assert(len==1 || len==2 ||len==4);
#endif
	if(0){//data cross the page boundary
		assert(0);
	}
	else{
		hwaddr_t hwaddr = page_translate(addr);
		return hwaddr_read(hwaddr,len);
	}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
	hwaddr_write(addr, len, data);
}
lnaddr_t seg_translate(swaddr_t addr,size_t len,uint32_t current_sreg){
	lnaddr_t res;
	if(cpu.cr0.protect_enable==0){
		res=addr;
		return res;
	}
#ifdef DEBUG
	assert(current_sreg==S_CS || current_sreg==S_DS || current_sreg==S_ES || current_sreg==S_SS);
#endif
	uint32_t addr15_0=cpu.DES[current_sreg].base15_0;
	uint32_t addr23_16=cpu.DES[current_sreg].base23_16;
	uint32_t addr31_24=cpu.DES[current_sreg].base31_24;
	res=addr+(addr15_0)+(addr23_16<<16)+(addr31_24<<24);
	return res;
}
uint32_t swaddr_read(swaddr_t addr, size_t len,uint32_t current_sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr,len,current_sreg);
	return lnaddr_read(lnaddr, len);
}

void swaddr_write(swaddr_t addr, size_t len, uint32_t data,uint32_t current_sreg) {
#ifdef DEBUG
	assert(len == 1 || len == 2 || len == 4);
#endif
	lnaddr_t lnaddr = seg_translate(addr,len,current_sreg);
	lnaddr_write(lnaddr, len, data);
}


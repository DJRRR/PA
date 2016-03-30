#include "common.h"
#include "nemu.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t read_cache(hwaddr_t,size_t);
void write_cache(hwaddr_t,size_t,uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
//	return read_cache(addr,len) & (~0u >> ((4-len)<<3));
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	dram_write(addr, len, data);
//	write_cache(addr,len,data);
}
hwaddr_t page_translate(lnaddr_t addr,size_t len){
	hwaddr_t res;
//	if(cpu.eip>=0x100c44){
//		printf("stop\n");
//	}
	if(cpu.cr0.protect_enable==1&&cpu.cr0.paging==1){//open page function
		//res=read_page_L1(addr,len);
		uint16_t dir=addr>>22;
		uint16_t page=(addr>>12)&0x3ff;
		uint16_t offset=addr&0xfff;
		uint32_t base=hwaddr_read((cpu.cr3.page_directory_base<<12)+dir*4,4);
		uint32_t page_data=hwaddr_read(base+page*4,4);
		res=offset+page_data;
	}
	else{
		res=(hwaddr_t)addr;
	}
	return res;
}

uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	//return hwaddr_read(addr, len);
#ifdef DEBUG
	assert(len==1 || len==2 ||len==4);
#endif
		hwaddr_t hwaddr = page_translate(addr,len);
		return hwaddr_read(hwaddr,len);
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
//	hwaddr_write(addr, len, data);
#ifdef DEBUG
	assert(len==1 || len==2 || len==4);
#endif
		hwaddr_t hwaddr = page_translate(addr,len);
		hwaddr_write(hwaddr,len,data);
}

lnaddr_t seg_translate(swaddr_t addr,size_t len,uint32_t current_sreg){
#ifdef DEBUG
	assert(current_sreg==S_CS || current_sreg==S_DS || current_sreg==S_ES || current_sreg==S_SS);
#endif
	lnaddr_t res;
	if(cpu.cr0.protect_enable==0){
		res=(lnaddr_t)addr;
		return res;
	}
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


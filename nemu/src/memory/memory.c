#include "common.h"
#include "nemu.h"
#include "../../../lib-common/x86-inc/mmu.h"
#include "stdlib.h"
#include "time.h"
#include "tlb2.h"
#include "device/mmio.h"

uint32_t dram_read(hwaddr_t, size_t);
void dram_write(hwaddr_t, size_t, uint32_t);
uint32_t read_cache(hwaddr_t,size_t);
void write_cache_new(hwaddr_t,size_t,uint32_t);
void write_cache(hwaddr_t,size_t,uint32_t);

/* Memory accessing interfaces */

uint32_t hwaddr_read(hwaddr_t addr, size_t len) {
	int judge=is_mmio(addr);
	if(judge==-1){
		return dram_read(addr, len) & (~0u >> ((4 - len) << 3));
	//	return read_cache(addr,len) & (~0u >> ((4 - len) << 3));
	}
	else{
		return mmio_read(addr,len,judge) & (~0u >> ((4 - len) << 3));
	}
}

void hwaddr_write(hwaddr_t addr, size_t len, uint32_t data) {
	int judge=is_mmio(addr);
	if(judge==-1){
		dram_write(addr, len, data);
	//	write_cache(addr,len,data);
	}
	else{
		mmio_write(addr,len,data,judge);
	}

}
hwaddr_t page_translate(lnaddr_t addr,size_t len){
	return read_page(addr);
}


uint32_t lnaddr_read(lnaddr_t addr, size_t len) {
	//return hwaddr_read(addr, len);
#ifdef DEBUG
	assert(len==1 || len==2 ||len==4);
#endif	
		uint32_t offset = addr & 0xfff;
		uint32_t new_len,res1,res2;
		if(cpu.cr0.protect_enable==0||cpu.cr0.paging==0){
			return hwaddr_read(addr,len);
		}
		hwaddr_t hwaddr = page_translate(addr,len);
//		hwaddr_t hwaddr = page_translate2(addr);
		if(len+offset>0x1000){
			new_len=0x1000-offset;
			res1=hwaddr_read(hwaddr,new_len);
			addr+=new_len;
			hwaddr=page_translate(addr,len);
			res2=hwaddr_read(hwaddr,len-new_len);
			res2=res2<<(new_len<<3);
			return res1+res2;
		}
		else{
			return hwaddr_read(hwaddr,len);
		}
}

void lnaddr_write(lnaddr_t addr, size_t len, uint32_t data) {
//	hwaddr_write(addr, len, data);
#ifdef DEBUG
	assert(len==1 || len==2 || len==4);
#endif	
		uint32_t offset=addr&0xfff;
		uint32_t new_len;
		if(cpu.cr0.protect_enable==0||cpu.cr0.paging==0){
			hwaddr_write(addr,len,data);
			return;
		}
		hwaddr_t hwaddr = page_translate(addr,len);
//		hwaddr_t hwaddr=page_translate(addr);
		if(len+offset>0x1000){
			new_len=0x1000-offset;
			hwaddr_write(hwaddr,new_len,data);
			addr+=new_len;
			hwaddr=page_translate(addr,len);
			data=data>>(new_len<<3);
			hwaddr_write(hwaddr,len-new_len,data);
		}
		else{
			hwaddr_write(hwaddr,len,data);
		}
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
	assert(res==addr);
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
/*	if(cpu.eip==0x80480ad){
		printf("here in memory.c\n");
	}*/
	lnaddr_t lnaddr = seg_translate(addr,len,current_sreg);
/*	if(cpu.eip==0x80480ad){
		printf("here in memory.c2\n");
	}*/
	lnaddr_write(lnaddr, len, data);
}


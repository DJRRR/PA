#include "common.h"
#include "nemu.h"
#include "../../../lib-common/x86-inc/mmu.h"
#include <memory/page.h>
#include <stdlib.h>
#include <time.h>


/*typedef struct{	
	unsigned int valid:1;
		unsigned int tag:20;
	unsigned int offset:12;
	unsigned int physical;
}Page_L1;

Page_L1 page_L1[64];
void init_page_L1(){
	int i=0;
	for(i=0;i<64;i++){
		page_L1[i].valid=0;
	}
}

bool find_page_L1(lnaddr_t addr,size_t len){
	int i=0;
	for(i=0;i<64;i++){
		if(page_L1[i].tag==(addr>>12)){
			if(page_L1[i].valid==1){
				return true;
			}
		}
	}
	return false;
}

hwaddr_t read_page_L1(lnaddr_t addr,size_t len){
	bool flag=find_page_L1(addr,len);
	hwaddr_t res;
	if(flag==true){//hit 
		bool check=false;
		int i=0;
		int way_i=-1;
		for(i=0;i<64;i++){
			if((addr>>12)==page_L1[i].tag&&page_L1[i].valid==1){
				way_i=i;
				check=true;
				break;
			}
		}
		if(check==false||way_i==-1){
			printf("page_L1 read error!\n");
			assert(0);
		}
		if((uint32_t)page_L1[way_i].offset+len<=4096){
			uint32_t offset=addr&0xfff;
		//	uint32_t temp1=(page_L1[way_i].physical>>12);
			res=offset+((page_L1[way_i].physical>>12)<<12);
			return res;
		}
		else{
			printf("page bound error!\n");
			assert(0);
		}
	}
	else{//miss
		srand((unsigned)time(0)+clock());
		int i_i=rand()%64;
		page_L1[i_i].valid=1;
		page_L1[i_i].offset=addr&0xfff;
		page_L1[i_i].tag=(addr>>12);
		unsigned int page_dic=(addr>>22);
		unsigned int page_table=(addr>>12)&0x3ff;
		unsigned int dic_addr=hwaddr_read((cpu.cr3.page_directory_base<<12)+page_dic*4,4)>>12;
		page_L1[i_i].physical=hwaddr_read((dic_addr<<12)+page_table*4,4);
	//	unsigned int temp2=page_L1[i_i].physical>>12;
		res=page_L1[i_i].offset+((hwaddr_read((dic_addr<<12)+page_table*4,4)>>12)<<12);
		return res;
	}
}
*/
typedef union{
	struct{
		unsigned int offset:12;//page
		unsigned int page:10;
		unsigned int dir:10;
	};
	struct{
		unsigned int offset_i:12;//tlb
		unsigned int tag:20;
	};
	unsigned int val;
}ln_addr;

typedef struct{
	bool valid;
	uint32_t tag;
	PTE pte;
}TLB[64];

TLB tlb;

void init_TLB(){
	int i;
	for(i=0;i<64;i++){
		tlb[i].valid=0;
	}
}

hwaddr_t read_page(lnaddr_t addr){
	if((cpu.cr0.protect_enable==0)||(cpu.cr0.paging==0)){
		return  addr;
	}
	ln_addr lnaddr;
	lnaddr.val=addr;
	int i;
	for(i=0;i<64;i++){
		if(tlb[i].valid&&(tlb[i].tag==lnaddr.tag)) break;
		if(!tlb[i].valid) break;
	}
	if(tlb[i].valid==0||i==64){
		if(i==64) i=addr%64;
		hwaddr_t pde_addr=(cpu.cr3.page_directory_base<<12)+lnaddr.dir*sizeof(PDE);//4
		PDE pde;
		pde.val=hwaddr_read(pde_addr,4);
		hwaddr_t pte_addr=(pde.page_frame<<12)+lnaddr.page*sizeof(PTE);//4
		PTE pte;
		pte.val=hwaddr_read(pte_addr,4);
		tlb[i].tag=lnaddr.tag;
		tlb[i].valid=1;
		tlb[i].pte=pte;
	}
	return ((tlb[i].pte.page_frame<<12)+lnaddr.offset);
}






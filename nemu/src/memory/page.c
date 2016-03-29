#include "common.h"
#include <memory/page.h>
#include <stdlib.h>
#include <time.h>


typedef struct{	
	unsigned int valid:1;
	union{
		unsigned int tag:20;
		struct{
			unsigned int page_table:10;
			unsigned int page_dic:10;
		};
	};
	unsigned int offset:12;
	unsigned int physical:20;
}Page_L1;

Page_L1 page_L1[32];
void init_page_L1(){
	int i=0;
	for(i=0;i<32;i++){
		page_L1[i].valid=0;
	}
}

bool find_page_L1(lnaddr_t addr,size_t len){
	unsigned int tag_i=(addr>>12)&0x000fffff;
	int i=0;
	for(i=0;i<32;i++){
		if(page_L1[i].tag==tag_i){
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
	unsigned int tag_i=(addr>>12)&0x000fffff;
	if(flag==true){//hit 
		bool check=false;
		int i=0;
		int way_i=-1;
		for(i=0;i<32;i++){
			if(tag_i==page_L1[i].tag&&page_L1[i].valid==1){
				way_i=i;
				check=true;
				break;
			}
		}
		if(check==false){
			printf("page_L1 read error!\n");
			assert(0);
		}
		if((uint32_t)page_L1[way_i].offset+len<=4096){
			uint32_t temp1=page_L1[way_i].physical<<12;
			res=page_L1[way_i].offset+temp1;
			return res;
		}
		else{
			printf("page bound error!\n");
			assert(0);
		}
	}
	else{//miss
		srand((unsigned)time(0)+clock());
		int i_i=rand()%32;
		page_L1[i_i].valid=1;
		page_L1[i_i].tag=(addr>>12)&0x000fffff;
		unsigned int page_dic=(page_L1[i_i].tag>>10)&0x3ff;
		unsigned int page_table=(page_L1[i_i].tag)&0x3ff;
		unsigned int dic_addr=hwaddr_read((cpu.cr3.page_directory_base<<12)+page_dic*4,4)&0xfffff000;
		unsigned int physic_addr=hwaddr_read(dic_addr+page_table*4,4);
		page_L1[i_i].physical=(physic_addr>>12)&0xfffff;
		unsigned int temp2=page_L1[i_i].physical<<12;
		res=page_L1[i_i].offset+temp2;
		return res;
	}
}










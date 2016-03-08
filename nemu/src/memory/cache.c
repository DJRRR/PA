#include <common.h>
#include<time.h>
#include <stdlib.h>

uint32_t dram_read(hwaddr_t,size_t);
uint32_t hwaddr_read(hwaddr_t,size_t);
void dram_write(hwaddr_t,size_t,uint32_t);

typedef struct{
	unsigned int valid:1;
	unsigned int tag:14;
	unsigned int index:7;
	unsigned int offset:6;
	uint8_t data[64];
}Cache_L1;

Cache_L1 cache_L1[128][8];//8-way 128zu

void init_cache_L1(){
	int i,j;
	for(i=0;i<128;i++){
		for(j=0;j<8;j++){
			cache_L1[i][j].valid=0;
		}
	}
}

bool find_cache_L1(hwaddr_t addr,size_t len){
//	unsigned int offset_i=addr&0x3f;
//	unsigned int index_i=(addr&0x1fc0)>>6;
	unsigned int index_i=(addr>>6)&0x7f;
//	unsigned int tag_i=(addr&0xfffe000)>>13;
	unsigned int tag_i=(addr>>13)&0x3fff;
	int i=0;
	for(;i<8;i++){
		if(tag_i==cache_L1[index_i][i].tag){
			if(cache_L1[index_i][i].valid==1){
				return true;
			}
		}
	}
	return false;
}

uint32_t read_cache_L1(hwaddr_t addr,size_t len){
	unsigned int offset_i=addr&0x3f;
//	unsigned int index_i=(addr&0x1fc0)>>6;
//	unsigned int tag_i=(addr&0xfffe000)>>13;
	unsigned int index_i=(addr>>6)&0x7f;
	unsigned int tag_i=(addr>>13)&0x3fff;
	bool flag=find_cache_L1(addr,len);
	int i,way_i=-1;
	bool check=false;
	if(flag==true){
	//	printf("here1\n");
		for(i=0;i<8;i++){
			if(cache_L1[index_i][i].tag==tag_i&&cache_L1[index_i][i].valid==1){
				way_i=i;
				check=true;
				break;
			}
		}
		if(check==false){
			printf("Read cache L1 error!\n");
		assert(0);
		}
		uint32_t res=cache_L1[index_i][way_i].data[offset_i+len-1];
		if(offset_i+len<=64){//check bound
			int j=0;
			for(j=len-2;j>=0;j--){
				res = (res<<8)+(cache_L1[index_i][way_i].data[offset_i+j]);//unchecked
			}
			return res;
		}
		else{
			printf("cache 1 bound error!\n");
			assert(0);
		}
	}
	else{
		srand((unsigned)time(NULL));
		int i_i=rand()%8;
		printf("TEST: %d\n",i_i);
		cache_L1[index_i][i_i].valid=1;
		cache_L1[index_i][i_i].tag=tag_i;
		cache_L1[index_i][i_i].index=index_i;
		hwaddr_t addr_new=addr-offset_i;
		int q=0;
		for(q=0;q<64;q++){
			cache_L1[index_i][i_i].data[q]=dram_read(addr_new+q,1);
		}
		return dram_read(addr_new,len)&(~0u>>((4-len)<<3));
	}
}



void  write_cache_L1(hwaddr_t addr, size_t len, uint32_t data){
	unsigned int offset_i=addr&0x3f;
//	unsigned int index_i=(addr&0x1fc0)>>6;
//	unsigned int tag_i=(addr&0xfffe000)>>13;
	unsigned int index_i=(addr>>6)&0x3f;
	unsigned int tag_i=(addr>>13)&0x3fff;
	bool flag=find_cache_L1(addr,len);
	uint32_t data_t=data;
	if(flag==true){//write through
		int way_i=-1;
		bool check=false;
		int i;
		for(i=0;i<8;i++){
			if(cache_L1[index_i][i].tag==tag_i&&cache_L1[index_i][i].valid==1){
				way_i=i;
				check=true;
				break;
			}
		}
		if(check==false){
			printf("False!\n");
			assert(0);
		}
		if(offset_i+len<=64){
			int j;
			for(j=len-1;j>=0;j--){
				cache_L1[index_i][way_i].data[j]=(data_t>>(j*8))&0xff;
			}
		}
		else{
			printf("write cache 1 bound error!\n");
			assert(0);
		}
	}
	else{//uncompleted
	}
	dram_write( addr,len,data);
}
				


			





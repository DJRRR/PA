#include <common.h>
#include<time.h>
#include <stdlib.h>

uint32_t dram_read(hwaddr_t,size_t);
uint32_t hwaddr_read(hwaddr_t,size_t);
void dram_write(hwaddr_t,size_t,uint32_t);
void hwaddr_write(hwaddr_t,size_t,uint32_t);

typedef struct{
	unsigned int valid:1;
	unsigned int tag:14;
	unsigned int index:7;
	unsigned int offset:6;
	uint8_t data[64];
}Cache_L1;

Cache_L1 cache_L1[8][128];//8-way

void init_cache_L1(){
	int i,j;
	for(i=0;i<8;i++){
		for(j=0;j<128;j++){
			cache_L1[i][j].valid=0;
		}
	}
}

bool find_cache_L1(hwaddr_t addr,size_t len){
//	unsigned int offset_i=addr&0x3f;
	unsigned int index_i=(addr&0x1fc0)>>6;
	unsigned int tag_i=(addr&0xfffe000)>>13;
	int i=0;
	for(;i<8;i++){
		if(tag_i==cache_L1[i][index_i].tag){
			if(cache_L1[i][index_i].valid==1){
				return true;
			}
		}
	}
	return false;
}

uint32_t read_cache_L1(hwaddr_t addr,size_t len){
	unsigned int offset_i=addr&0x3f;
	unsigned int index_i=(addr&0x1fc0)>>6;
	unsigned int tag_i=(addr&0xfffe000)>>13;
	bool flag=find_cache_L1(addr,len);
	int i,way_i=-1;
	bool check=false;
	if(flag==true){
		printf("here1\n");
		for(i=0;i<8;i++){
			if(cache_L1[i][index_i].tag==tag_i){
				way_i=i;
				check=true;
				break;
			}
		}
		if(check==false){
		//	printf("Read cache L1 error!\n");
		assert(0);
		}
		uint32_t res=0x00000000;
		if(offset_i+len<=64){//check bound
			int j=0;
			for(j=0;j<len;j++){
				res = (res<<8)|cache_L1[way_i][index_i].data[offset_i+j];//unchecked
			}
			return res;
		}
		else{
			printf("cache 1 bound error!\n");
			assert(0);
		}
	}
	else{
		int num;
		srand((unsigned)time(NULL));
		num=rand()%128;
		int i_i=(tag_i)%8;
		cache_L1[i_i][num].valid=1;
		cache_L1[i_i][num].tag=tag_i;
		cache_L1[i_i][num].index=num;
		cache_L1[i_i][num].data[0]=dram_read(addr&0xfffffff8,8);
		return dram_read(addr,len);
	}
}






			





#include <common.h>
#include<time.h>
#include <stdlib.h>

typedef struct{
	unsigned int valid:1;
	unsigned int tag:14;
	unsigned int index:7;
	unsigned int offset:6;
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



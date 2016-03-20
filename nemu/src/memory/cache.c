#include <common.h>
#include<time.h>
#include <stdlib.h>
#include <memory/cache.h>

uint32_t dram_read(hwaddr_t,size_t);
uint32_t hwaddr_read(hwaddr_t,size_t);
void hwaddr_write(hwaddr_t , size_t , uint32_t);
void dram_write(hwaddr_t,size_t,uint32_t);

typedef struct{
	unsigned int valid:1;
	unsigned int tag:14;
	unsigned int index:7;
	unsigned int offset:6;
	uint8_t data[64];
}Cache_L1;

Cache_L1 cache_L1[128][8];//8-way 128zu

typedef struct{
	unsigned int valid:1;
	unsigned int dirty:1;
	unsigned int tag:9;
	unsigned int index:12;
	unsigned int offset:6;
	unsigned int data[64];
}Cache_L2;

Cache_L2 cache_L2[4096][16];//16-way 4096zu


void init_cache_L1(){
	int i,j;
	for(i=0;i<128;i++){
		for(j=0;j<8;j++){
			cache_L1[i][j].valid=0;
		}
	}
}
void init_cache_L2(){
	int i,j;
	for(i=0;i<4096;i++){
		for(j=0;j<16;j++){
			cache_L2[i][j].valid=0;
		}
	}
}
bool find_cache_L1(hwaddr_t addr,size_t len){
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

bool find_cache_L2(hwaddr_t addr,size_t len){
	unsigned int index_i=(addr>>6)&0xfff;
	unsigned int tag_i=(addr>>(6+12))&0x1ff;
	int i=0;
	for(;i<16;i++){
		if(tag_i==cache_L2[index_i][i].tag){
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
	//	uint32_t res=cache_L1[index_i][way_i].data[offset_i+len-1];
		uint32_t res=0;
		if(offset_i+len<=64){//check bound
		//	res=cache_L1[index_i][way_i].data[offset_i+len-1];
			int j=0;
			for(j=len-1;j>=0;j--){
				res = (res<<8)+(cache_L1[index_i][way_i].data[offset_i+j]);//unchecked
			}
			return res;
		}
		else{
		//	printf("read cache 1 bound error!\n");
		//	assert(0);
		//    unalign_rw(addr,len);
		   // int max_block=64-offset_i;
			int begin=63;
		//	uint32_t res_over=cache_L1[index_i][way_i].data[begin];
		    uint32_t res_over=0;
			int m=0;
			for(m=begin;m>=offset_i;m--){
				res_over = (res_over<<8)+cache_L1[index_i][way_i].data[m];
			}
			uint32_t res_over2=hwaddr_read(((addr+0x40)>>6)<<6,offset_i+len-64);//why?
			res_over=(res_over2<<((64-offset_i)*8))+res_over;
			return res_over;
		}
	}
	else{
		srand((unsigned)time(0)+clock());
		int i_i=rand()%8;
	//	printf("TEST: %d\n",i_i);
		cache_L1[index_i][i_i].valid=1;
		cache_L1[index_i][i_i].tag=tag_i;
		cache_L1[index_i][i_i].index=index_i;
		hwaddr_t addr_new=addr-offset_i;
//		printf("0x%X\n",addr_new);
		int q=0;
		for(q=0;q<=63;q++){
			cache_L1[index_i][i_i].data[q]=dram_read(addr_new+q,1)&0xff;
		//	system("pause");
		}	
		return hwaddr_read(addr,len);
	}
}



void  write_cache_L1(hwaddr_t addr, size_t len, uint32_t data){
	unsigned int offset_i=addr&0x3f;
//	unsigned int index_i=(addr&0x1fc0)>>6;
//	unsigned int tag_i=(addr&0xfffe000)>>13;
	unsigned int index_i=(addr>>6)&0x7f;
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
			for(j=len-1;j>=0;j--){//change
				cache_L1[index_i][way_i].data[j+offset_i]=(data_t>>(j*8))&0xff;//change
			}
		}
		else{
		//	printf("write cache 1 bound error!\n");
		//	assert(0);
		   // int max_block=64-offset_i;
		   int end=63;
		   int m;
		   for(m=offset_i;m<=end;m++){
			   cache_L1[index_i][way_i].data[m]=data&0xff;
			   data >>= 8;
		   }
		   hwaddr_write(((addr+0x40)>>6)<<6,offset_i+len-64,data);		    
		    
		}
	}
	else{
	dram_write( addr,len,data);
	}
}
				


			





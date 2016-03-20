
#include "common.h"
#include <memory/cache.h>
#include <stdlib.h>
#include <time.h>

unsigned int buf_1f2[4];//tag_j(0)+index_j(1)+offset_j(2)+way_j(3)

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
			cache_L2[i][j].dirty=0;
		}
	}
}
void rewrite_cache_L1_fdram(hwaddr_t addr,unsigned int index_i,unsigned int tag_i,unsigned int offset_i){//right
	srand((unsigned)time(0)+clock());
	int i_i=rand()%8;
	cache_L1[index_i][i_i].valid=1;
	cache_L1[index_i][i_i].tag=tag_i;
	cache_L1[index_i][i_i].index=index_i;
	hwaddr_t addr_new=addr-offset_i;
	int q=0;
	for(q=0;q<16;q++){
		cache_L1[index_i][i_i].data_buf[q]=dram_read(addr_new+q*4,4);
	}
}

void rewrite_cache_L1_fcache2(hwaddr_t addr,unsigned int index_i,unsigned int tag_i,unsigned int offset_i){
	srand((unsigned)time(0)+clock());
	int i_i=rand()%8;
	cache_L1[index_i][i_i].valid=1;
	cache_L1[index_i][i_i].index=index_i;
	cache_L1[index_i][i_i].tag=tag_i;
	unsigned int index_j=(addr>>6)&0xfff;
	unsigned int tag_j=(addr>>18)&0x1ff;
//	hwaddr_t addr_new=addr-offset_i;
	int num=0;
	bool check2=false;
	int way_j=0;
	for(num=0;num<16;num++){
		if(cache_L2[index_j][num].tag==tag_j&&cache_L2[index_j][num].valid==1){
			way_j=num;
			check2=true;
			break;
		}
	}
	if(check2==false){
		printf("Flase read cache1 from cache2!\n");
		assert(0);
	}
	int cnt=0;
	for(cnt=0;cnt<64;cnt++){
		cache_L1[index_i][i_i].data[cnt]=cache_L2[index_j][way_j].data[cnt];
	}
	buf_1f2[0]=tag_j;//update buf
	buf_1f2[1]=index_j;
	buf_1f2[2]=offset_i;
	buf_1f2[3]=way_j;
}

void rewrite_cache_L2_fdram(hwaddr_t addr){
	unsigned int index_j=(addr>>6)&0xfff;
	unsigned int tag_j=(addr>>18)&0x1ff;
	unsigned int offset_j=addr&0x3f;
	srand((unsigned)time(0)+clock());
	int x=rand()%16;
	if(cache_L2[index_j][x].valid==1&&cache_L2[index_j][x].dirty==1){//write back
		cache_L2[index_j][x].dirty=0;
		uint32_t back_addr=(cache_L2[index_j][x].tag<<18)+(cache_L2[index_j][x].index<<6);
		int i_back=0;
		for(i_back=0;i_back<64;i_back++){
			dram_write(back_addr+i_back,1,cache_L2[index_j][x].data[i_back]);
		}
	}
	cache_L2[index_j][x].valid=1;
	cache_L2[index_j][x].tag=tag_j;
	cache_L2[index_j][x].index=index_j;
	int cnt2=0;
	addr=addr-offset_j;
	for(cnt2=0;cnt2<64;cnt2++){//read cache2 from dram
		cache_L2[index_j][x].data[cnt2]=dram_read(addr+cnt2,1);
	}
//	cache_L2[index_j][x].dirty=0;
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
	unsigned int tag_i=(addr>>18)&0x1ff;
	int i=0;
	for(;i<16;i++){
		if(tag_i==cache_L2[index_i][i].tag){
			if(cache_L2[index_i][i].valid==1){
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
	bool flag1=find_cache_L1(addr,len);
	bool flag2=find_cache_L2(addr,len);
	int i,way_i=-1;
	bool check1=false;
	if(flag1==true){// if addr in cache_L1
	//	printf("read case 1\n");
		for(i=0;i<8;i++){
			if(cache_L1[index_i][i].tag==tag_i&&cache_L1[index_i][i].valid==1){
				way_i=i;
				check1=true;
				break;
			}
		}
		if(check1==false){
			printf("Read cache L1 error!\n");
		    assert(0);
		}
		uint32_t res=0;
		if(offset_i+len<=64){//check bound of cache1
			int j=0;
			for(j=len-1;j>=0;j--){
				res = (res<<8)+(cache_L1[index_i][way_i].data[offset_i+j]);//unchecked
			}
			return res;
		}
		else{
		//	printf("read cache 1 bound error!\n");
		//	assert(0);
		//    unalign_rw(addr,len);//???
			int begin=63;
		    uint32_t res_over=0;
			int m=0;
			for(m=begin;m>=offset_i;m--){
				res_over = (res_over<<8)+cache_L1[index_i][way_i].data[m];
			}
			uint32_t res_over2=hwaddr_read((addr+0x40)&0xffffffc0,offset_i+len-64);
			res_over=(res_over2<<((64-offset_i)*8))+res_over;
			return res_over;
		}
	}
	else if(flag2==true){//not in cache 1 but in cache 2 then read cache1 from cache2 and read data from cache2
	//	printf("read case 2\n");
		rewrite_cache_L1_fcache2(addr,index_i,tag_i,offset_i);
	//	unsigned int tag_j=buf_1f2[0];
		unsigned int index_j=buf_1f2[1];
		unsigned int offset_j=buf_1f2[2];
		unsigned int way_j=buf_1f2[3];
		uint32_t res2=0;
		if(offset_j+len<=64){//check bound of cache2
			int j2=0;
			for(j2=len-1;j2>=0;j2--){
				res2=(res2<<8)+cache_L2[index_j][way_j].data[offset_j+j2];
			}
			return res2;
		}
		else{//over bound
			//printf("read cache 2 bound error!\n");
			//assert(0);
			int begin2=63;
			uint32_t res_over_=0;
			int m2=0;
			for(m2=begin2;m2>=offset_j;m2--){
				res_over_=(res_over_<<8)+cache_L2[index_j][way_j].data[m2];
			}
			uint32_t res_over_2=hwaddr_read((addr+0x40)&0xffffffc0,offset_j+len-64);
			res_over_=(res_over_2<<((64-offset_j)*8))+res_over_;
			return res_over_;
		}
        
	}
	else{//not in cache1 not in cache2 then read cache2 from dram and read cache1 from cache2
	//	printf("read case 3\n");
		rewrite_cache_L2_fdram(addr);
		rewrite_cache_L1_fcache2(addr,index_i,tag_i,offset_i);
		return dram_read(addr,len);


	}
}



void  write_cache_L1(hwaddr_t addr, size_t len, uint32_t data){
	unsigned int offset_i=addr&0x3f;
//	unsigned int index_i=(addr&0x1fc0)>>6;
//	unsigned int tag_i=(addr&0xfffe000)>>13;
	unsigned int index_i=(addr>>6)&0x7f;
	unsigned int tag_i=(addr>>13)&0x3fff;
	unsigned int tag_j=(addr>>18)&0x1ff;
	unsigned int index_j=(addr>>6)&0xfff;
	bool flag1=find_cache_L1(addr,len);
	bool flag2=find_cache_L2(addr,len);
	uint32_t data_t=data;
	uint32_t data2=data;
	uint32_t data3=data;
 	if(flag1==true){//in cache1 then write cache1 write cache2 set dirty=1
	//	printf("write case 1\n");
		int way_i=-1;//write cache 1
		bool check1=false;
		int i;
		for(i=0;i<8;i++){
			if(cache_L1[index_i][i].tag==tag_i&&cache_L1[index_i][i].valid==1){
				way_i=i;
				check1=true;
				break;
			}
		}
		if(check1==false){
			printf("write cache1 False!\n");
			assert(0);
		}
		if(offset_i+len<=64){//cache 1 in bound
			int j;
			for(j=len-1;j>=0;j--){//change
				cache_L1[index_i][way_i].data[j+offset_i]=(data_t>>(j*8));//change
			}
		}
		else{//cache 1 over bound
		//	printf("write cache 1 bound error!\n");
		//	assert(0);
		   int end=63;
		   int m;
		   for(m=offset_i;m<=end;m++){
			   cache_L1[index_i][way_i].data[m]=data;
			   data >>= 8;
		   }
		   hwaddr_write((addr+0x40)&0xffffffc0,offset_i+len-64,data);		    
		    
		}
		int way_j=-1;
		bool check2=false;
		int i_j;
		for(i_j=0;i_j<16;i_j++){
			if(cache_L2[index_j][i_j].tag==tag_j&&cache_L2[index_j][i_j].valid==1){
				way_j=i_j;
				check2=true;
				break;
			}
		}
		if(check2==false){
			printf("write cache2 False!\n");
			assert(0);
		}
		if(offset_i+len<=64){//cache 2 in bound
			int j_j;
			for(j_j=len-1;j_j>=0;j_j--){
				cache_L2[index_j][way_j].data[j_j+offset_i]=(data>>(j_j*8));
			}
		}
		else{//cache 2 over bound
			int end_j=63;
			int m_j;
			for(m_j=offset_i;m_j<=end_j;m_j++){
				cache_L2[index_j][way_j].data[m_j]=data2;
				data2 >>= 8;
			}
			hwaddr_write((addr+0x40)&0xffffffc0,offset_i+len-64,data2);
		}
	//	cache_L2[index_j][way_j].dirty=1;
	}
	else if(flag2==true){//not in cache1 but in cache2 then write cache 2 set dirty =1
	//	printf("write case 2\n");
		int way_f;
		int i_f;
		bool check3=false;
		for(i_f=0;i_f<16;i_f++){
			if(cache_L2[index_j][i_f].tag==tag_j&&cache_L2[index_j][i_f].valid==1){
				way_f=i_f;
				check3=true;
				break;
			}
		}
		if(check3==false){
			printf("write cache2 false 2!\n");
			assert(0);
		}
		if(offset_i+len<=64){
			int j_f;
			for(j_f=len-1;j_f>=0;j_f--){
				cache_L2[index_j][way_f].data[j_f+offset_i]=(data>>(j_f*8));
			}
		}
		else{
			int end_f=63;
			int m_f;
			for(m_f=offset_i;m_f<=end_f;m_f++){
				cache_L2[index_j][way_f].data[m_f]=data3;
				data3 >>= 8;
			}
			hwaddr_write((addr+0x40)&0xffffffc0,offset_i+len-64,data3);
		}
		cache_L2[index_j][way_f].dirty=1;
	}
	else{//not in cache 1 and not in cache2 then write dram then write cache2 and set dirty=0;
	//	printf("write case 3\n");
		dram_write(addr,len,data);
		int i_g;
		srand((unsigned)time(0)+clock());
		i_g=rand()%16;
		uint32_t addr_g=(cache_L2[index_j][i_g].tag<<18)+(cache_L2[index_j][i_g].index<<6);
		if(cache_L2[index_j][i_g].valid==1&&cache_L2[index_j][i_g].dirty==1){
			int i_gback=0;
			for(i_gback=0;i_gback<64;i_gback++){
				dram_write(addr_g+i_gback,1,cache_L2[index_j][i_g].data[i_gback]);
			}
		}
		cache_L2[index_j][i_g].valid=1;
		cache_L2[index_j][i_g].dirty=0;
		cache_L2[index_j][i_g].index=index_j;
		cache_L2[index_j][i_g].tag=tag_j;
		cache_L2[index_j][i_g].offset=offset_i;
		int xx=0;
		hwaddr_t addr_2g=addr-offset_i;
		for(xx=0;xx<64;xx++){
			cache_L2[index_j][i_g].data[xx]=dram_read(addr_2g+xx,1);
		}
	}
}
				


			





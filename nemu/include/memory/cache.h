#include "common.h"

typedef struct {
	unsigned int valid:1;
	unsigned int tag:14;
	unsigned int index:7;
	unsigned int offset:6;
	union{
		uint8_t data[64];
		uint32_t data_buf[16];
	};
}Cache_L1;

typedef struct {
	unsigned int valid:1;
	unsigned int dirty:1;
	unsigned int tag:9;
	unsigned int offset:6;
	union{
		uint8_t data[64];
		uint32_t data_buf[16];
	};
}Cache_L2;

Cache_L1 cache_L1[128][8];//8-way 128 zu
Cache_L2 cache_L2[4096][16];//16-way 4096zu

void init_cache_L1();
void init_cache_L2();
bool find_cache_L1(hwaddr_t addr,size_t len);
bool find_cache_L2(hwaddr_t addr,size_t len);
uint32_t read_cache_L1(hwaddr_t addr,size_t len);
void write_cache_L1(hwaddr_t addr,size_t len,uint32_t data);
extern uint32_t dram_read(hwaddr_t,size_t);
extern uint32_t hwaddr_read(hwaddr_t,size_t);
extern void hwaddr_write(hwaddr_t,size_t,uint32_t);
extern void dram_write(hwaddr_t,size_t,uint32_t);

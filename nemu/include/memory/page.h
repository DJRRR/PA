#include "common.h"
#include "nemu.h"

extern uint32_t hwaddr_read(hwaddr_t,size_t);
void init_page_L1();
bool find_page_L1(lnaddr_t addr,size_t len);
hwaddr_t read_page_L1(lnaddr_t addr,size_t len);

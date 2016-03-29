#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "smsw-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "smsw-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "smsw-template.h"
#undef DATA_BYTE

make_helper_v(smsw_rm)

#include "prefix/prefix.h"

#include "data-mov/mov.h"
#include "data-mov/xchg.h"
#include "data-mov/push.h"//PA2.1
#include "data-mov/pop.h"//PA2.2
#include "data-mov/leave.h"//PA2.2
#include "data-mov/jmp.h"
#include "data-mov/ret.h"
#include "data-mov/movsx.h"//PA2.3
#include "data-mov/movzx.h"//PA2.3
#include "data-mov/cltd.h"
#include "data-mov/movs.h"
#include "data-mov/cld.h"
#include "data-mov/stos.h"
#include "data-mov/cmps.h"
//#include "data-mov/lea.h"

#include "arith/sub.h"//PA2.1
#include "arith/dec.h"
#include "arith/inc.h"
#include "arith/neg.h"
#include "arith/imul.h"
#include "arith/mul.h"
#include "arith/idiv.h"
#include "arith/div.h"
#include "arith/add.h"//PA2.2
#include "arith/adc.h"//PA2.3
#include "arith/sbb.h"//PA2.3

#include "logic/and.h"//OK
#include "logic/or.h"
#include "logic/not.h"
#include "logic/xor.h"
#include "logic/sar.h"
#include "logic/shl.h"
#include "logic/shr.h"
#include "logic/shrd.h"
#include "logic/cmp.h"//PA2.1
#include "logic/call.h"//PA2.1
#include "logic/test.h"//PA2.1
#include "logic/je.h"//PA2.1
#include "logic/jbe.h"//PA2.2
#include "logic/jle.h"//PA2.3
#include "logic/jne.h"//PA2.3
#include "logic/seta.h"//PA2.3
#include "logic/setne.h"//PA2.3
#include "logic/jg.h"//PA2.3
#include "logic/jl.h"//PA2.3
#include "logic/ja.h"//PA2.3
#include "logic/jge.h"//PA2.3

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"

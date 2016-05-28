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
#include "data-mov/cmov.h"//PA2.3
#include "data-mov/lgdt.h"
#include "data-mov/lidt.h"
//#include "data-mov/smsw.h"
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
#include "logic/seta.h"//PA2.3
#include "logic/setne.h"//PA2.3
#include "logic/setl.h"
#include "logic/sete.h"
#include "logic/setbe.h"
#include "logic/setg.h"
#include "logic/setge.h"
#include "logic/setle.h"
#include "logic/std.h"//PA3.3
#include "logic/jcc.h"

#include "string/rep.h"

#include "misc/misc.h"

#include "special/special.h"

#include "int/int.h"//PA4.1
#include "int/pusha.h"//PA4.1
#include "int/popa.h"//PA4.1
#include "int/cli.h"//PA4.1
#include "int/iret.h"//PA4.1
#include "int/in.h"//PA4.2
#include "int/out.h"//PA4.2
#include "int/bt.h"//PA4.2

#ifndef MACHINE_H
#define MACHINE_H

/* Use the macro definitions of nop(), brk() and wait() in the r_cg_macrodriver.h to avoid conflictions. */
#ifndef nop
#define nop()  asm("nop;")
#endif
#ifndef brk
#define brk()  asm("brk;")
#endif
#ifndef wait
#define wait() asm("wait;")
#endif

/* Use the implementations in the CCRXmachine2.h for others. */
#include "CCRXmachine2.h"

#endif /* MACHINE_H */

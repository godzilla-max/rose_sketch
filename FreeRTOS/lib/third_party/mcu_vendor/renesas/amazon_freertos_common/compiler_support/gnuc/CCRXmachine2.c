/*
Use switch case function instead of RX INT instruction execution on stack.
#define DONT_EXECUTE_RX_INT_INSTRUCTION_ON_STACK
*/

#ifdef DONT_EXECUTE_RX_INT_INSTRUCTION_ON_STACK

void ccrx_machine_h_int_exception_switch_case_function(signed long num);

#define INTNUM(num)\
    __builtin_rx_int(num)

#define INTN(N)\
    case N:\
    INTNUM(N);\
    break;

#define INTN5(N)\
    INTN(N)\
    INTN(N+1)\
    INTN(N+2)\
    INTN(N+3)\
    INTN(N+4)

#define INTN10(N)\
    INTN5(N)\
    INTN5(N+5)

#define INTN50(N)\
    INTN10(N)\
    INTN10(N+10)\
    INTN10(N+20)\
    INTN10(N+30)\
    INTN10(N+40)

void ccrx_machine_h_int_exception_switch_case_function(signed long num)
{
    switch(num)
    {
    INTN50(0)
    INTN50(50)
    INTN50(100)
    INTN50(150)
    INTN50(200)
    INTN5(250)
    INTN(255)
    default:
    INTNUM(255);
    }
}

#endif /* DONT_EXECUTE_RX_INT_INSTRUCTION_ON_STACK */

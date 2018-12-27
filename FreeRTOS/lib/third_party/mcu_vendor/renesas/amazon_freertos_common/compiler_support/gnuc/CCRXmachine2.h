/*
License:
The MIT License will be used.
https://opensource.org/licenses/MIT
*/

/*
Docments:
https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html
and
https://gcc.gnu.org/onlinedocs/gcc/Using-Assembly-Language-with-C.html
https://gcc.gnu.org/onlinedocs/gcc/Basic-Asm.html
https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html
(https://gcc.gnu.org/onlinedocs/gcc/Extended-Asm.html)
https://gcc.gnu.org/onlinedocs/gcc/Constraints.html
https://gcc.gnu.org/onlinedocs/gcc/Simple-Constraints.html
https://gcc.gnu.org/onlinedocs/gcc/Modifiers.html
https://gcc.gnu.org/onlinedocs/gcc/Asm-Labels.html
https://gcc.gnu.org/onlinedocs/gcc/Explicit-Register-Variables.html
https://gcc.gnu.org/onlinedocs/gcc/Size-of-an-asm.html
and
https://gcc.gnu.org/onlinedocs/gcc/Alternate-Keywords.html
https://gcc.gnu.org/onlinedocs/gcc/Local-Register-Variables.html
https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
and
https://stackoverflow.com/questions/15819794/when-to-use-earlyclobber-constraint-in-extended-gcc-inline-assembly
https://stackoverflow.com/questions/31718637/determine-optimization-level-in-preprocessor
https://github.com/ARMmbed/mbed-os/blob/master/cmsis/TARGET_CORTEX_M/
and
https://gcc-renesas.com/ja/rx/rx-latest-source-code/
rx_gcc4.8.4_2018q1/gcc/gcc/config/rx/rx.c
   Handles the insertion of a single operand into the assembler output.
   The %<letter> directives supported are:

     %A  Print an operand without a leading # character.
     %B  Print an integer comparison name.
     %b  Print a bit number based on a single set or cleared bit.   
     %C  Print a control register name.
     %F  Print a condition code flag name.
     %G  Register used for small-data-area addressing
     %H  Print high part of a DImode register, integer or address.
     %L  Print low part of a DImode register, integer or address.
     %N  Print the negation of the immediate value.
     %P  Register used for PID addressing
     %Q  If the operand is a MEM, then correctly generate
         register indirect or register relative addressing.
     %R  Like %Q but for zero-extending loads. 
     %S  %Q with unsigned mode.
     %U  print QI unsigned constant
     %V  print HI unsigned constant
*/

/*
Use switch case function instead of RX INT instruction execution on stack.
#define DONT_EXECUTE_RX_INT_INSTRUCTION_ON_STACK
*/

#ifndef CCRXMACHINE2_H
#define CCRXMACHINE2_H

#ifndef   __ASM
  #define __ASM                 __asm__
#endif
#ifndef   __INLINE
  #define __INLINE              __inline__
#endif
#ifndef   __STATIC_INLINE
  #define __STATIC_INLINE       static __inline__
#endif
#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE  __attribute__((always_inline)) static __inline__
#endif
#ifndef   __NO_RETURN
  #define __NO_RETURN           __attribute__((__noreturn__))
#endif
#ifndef   __USED
  #define __USED                __attribute__((used))
#endif
#ifndef   __WEAK
  #define __WEAK                __attribute__((weak))
#endif
#ifndef   __PACKED
  #define __PACKED              __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_STRUCT
  #define __PACKED_STRUCT       struct __attribute__((packed, aligned(1)))
#endif
#ifndef   __PACKED_UNION
  #define __PACKED_UNION        union __attribute__((packed, aligned(1)))
#endif

#ifndef max
#define max(data1, data2) ccrx_machine_h_max(data1, data2)
#endif
__STATIC_FORCEINLINE signed long ccrx_machine_h_max(signed long data1, signed long data2)
{
/* CC-RX V2.03 -optimize=2
        MAX R2, R1
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MAX %[R2], %[R1]"
        : /* OutputOperands */
            [R1] "+r" (data1)
        : /* InputOperands */
            [R2] "r" (data2)
    );
    return data1;
/* GNURX 2018q1 -O2
  16 0000 FC 13 21                      MAX r2, r1
  22 0003 02                            rts
*/
}

#ifndef min
#define min(data1, data2) ccrx_machine_h_min(data1, data2)
#endif
__STATIC_FORCEINLINE signed long ccrx_machine_h_min(signed long data1, signed long data2)
{
/* CC-RX V2.03 -optimize=2
        MIN R2, R1
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MIN %[R2], %[R1]"
        : /* OutputOperands */
            [R1] "+r" (data1)
        : /* InputOperands */
            [R2] "r" (data2)
    );
    return data1;
/* GNURX 2018q1 -O2
  35 0004 FC 17 21                      MIN r2, r1
  41 0007 02                            rts
*/
}

#ifndef revl
#define revl(data) ccrx_machine_h_revl(data)
#endif
__STATIC_FORCEINLINE signed long revl(signed long data)
{
/* CC-RX V2.03 -optimize=2
        REVL R1, R1
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "REVL %[R1], %[R1]"
        : /* OutputOperands */
            [R1] "+r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
  54 0008 FD 67 11                      REVL r1, r1
  60 000b 02                            rts
*/
}

#ifndef revw
#define revw(data) ccrx_machine_h_revw(data)
#endif
__STATIC_FORCEINLINE signed long revw(signed long data)
{
/* CC-RX V2.03 -optimize=2
        REVW R1, R1
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "REVW %[R1], %[R1]"
        : /* OutputOperands */
            [R1] "+r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
  73 000c FD 65 11                      REVW r1, r1
  79 000f 02                            rts
*/
}

#ifndef xchg
#define xchg(data1, data2) ccrx_machine_h_xchg(data1, data2)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_xchg(signed long *data1, signed long *data2)
{
/* CC-RX V2.03 -optimize=2
        MOV.L [R1], R14
        XCHG [R2].L, R14
        MOV.L R14, [R1]
        RTS
*/
    register signed long temp;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MOV.L [%[R1]], %[R14]"         "\n\t"
        "XCHG [%[R2]].L, %[R14]"        "\n\t"
        "MOV.L %[R14], [%[R1]]"         "\n\t"
        : /* OutputOperands */
            [R14] "=&r" (temp),
            /***/ "+m" (*data1),
            /***/ "+m" (*data2)
        : /* InputOperands */
            [R1] "r" (data1),
            [R2] "r" (data2)

    );
    return;
/* GNURX 2018q1 -O2
  92 0010 EC 15                         MOV.L [r1], r5
  93 0012 06 A0 10 25                   XCHG [r2].L, r5
  94 0016 E3 15                         MOV.L r5, [r1]
  97 0018 02                            rts
*/
}

#ifndef rmpab
#define rmpab(init, count, addr1, addr2) ccrx_machine_h_rmpab(init, count, addr1, addr2)
#endif
__STATIC_FORCEINLINE long long ccrx_machine_h_rmpab(long long init, unsigned long count, signed char *addr1, signed char *addr2)
{
/* CC-RX V2.03 -optimize=2
        ARG init = R2:R1
        ARG count = R3
        ARG addr1 = R4
        ARG addr2 = 04H[R0](08H[R0] after push R6)
        RET R2:R1

        PUSH.L R6
        MOV.L R1, R14
        MOV.L R2, R5
        MOV.L 08H[R0], R2
        SHAR #1FH, R5, R6
        MOV.L R4, R1
        MOV.L R14, R4
        RMPA.B
        MOV.L R5, R2
        MOV.L R4, R1
        RTSD #04H, R6-R6
*/
    register long long _r5r4 __ASM("r4") = init;
    register signed char *_r1 __ASM("r1") = addr1;
    register signed char *_r2 __ASM("r2") = addr2;
    register unsigned long _r3 __ASM("r3") = count;
#ifdef __OPTIMIZE__
    /* In case of other than -O0, we assign r6. */
    register signed long _rS __ASM("r6") = (signed long)(init >> 63);
#else
    /* In case of -O0, we assign r8 because r6 and r7 are reserved for the frame pointer and arg(?) pointer. */
    register signed long _rS __ASM("r8") = (signed long)(init >> 63);
#endif
    __ASM volatile
    (
        /* AssemblerTemplate */
#ifdef __OPTIMIZE__
        "RMPA.B"                        "\n\t"
#else
        "XCHG R6, R8"                   "\n\t"
        "RMPA.B"                        "\n\t"
        "XCHG R6, R8"                   "\n\t"
#endif
        : /* OutputOperands */
            /**/ "+r" (_rS),
            /**/ "+r" (_r5r4),
            /**/ "+r" (_r3)
        : /* InputOperands */
            /**/ "r" (_r1),
            /**/ "r" (_r2),
            /**/ "m" (*(signed char (*)[count]) addr1),
            /**/ "m" (*(signed char (*)[count]) addr2)
        : /* Clobbers */
            /*"r1",*/ /* This causes a compile error: asm-specifier for variable '_r1' conflicts with asm clobber list */
            /*"r2" */ /* This causes a compile error: asm-specifier for variable '_r2' conflicts with asm clobber list */
    );
    __ASM volatile
    (
        /* AssemblerTemplate */
        ""/*"NOP"*/                     "\n\t"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            /* No inputs. */
        : /* Clobbers */
            "r1",
            "r2"
        /* This asm statement can make the compiler to recognize that r1 and r2 are not equivalent to addr1 and addr2. */
        /* For example, after replacing 'return _r5r4;' --> 'return *addr1 * *addr2;' tentatively, code are different. */
/* GNURX 2018q1 -O2
In case of WITHOUT this asm statement, it is recognized that r1 and r2 are equivalent to addr1 and addr2 mistakenly.
     0019 7E A6                         push.l  r6
     001b 60 40                         sub     #4, r0
     001d EF 25                         mov.L   r2, r5
     001f A8 8A                         mov.L   12[r0], r2
     0021 FC 43 41                      xchg    r4, r1
     0024 FD BF 56                      shar    #31, r5, r6
     0027 7F 8C                         RMPA.B
 return *addr1 * *addr2;
     0029 CC 15                         mov.B   [r1], r5
     002b CC 21                         mov.B   [r2], r1
     002d 4F 51                         mul     r5, r1
     002f FD BF 12                      shar    #31, r1, r2
     0032 3F 66 02                      rtsd    #8, r6-r6
In case of WITH this asm statement, it is recognized that r1 and r2 are not equivalent to addr1 and addr2 correctly.
     0019 7E A6                         push.l  r6
     001b 60 40                         sub     #4, r0
     001d EF 25                         mov.L   r2, r5
     001f ED 0E 03                      mov.L   12[r0], r14
     0022 EF 4F                         mov.L   r4, r15
     0024 EF 14                         mov.L   r1, r4
     0026 EF E2                         mov.L   r14, r2
     0028 EF F1                         mov.L   r15, r1
     002a FD BF 56                      shar    #31, r5, r6
     002d 7F 8C                         RMPA.B
 return *addr1 * *addr2;
     002f CC F1                         mov.B   [r15], r1
     0031 06 0C E1                      mul     [r14].B, r1
     0034 FD BF 12                      shar    #31, r1, r2
     0037 3F 66 02                      rtsd    #8, r6-r6
*/
    );
    return _r5r4;
/* GNURX 2018q1 -O2
 108 0019 7E A6                         push.l  r6
 110 001b 60 40                         sub     #4, r0
 113 001d EF 25                         mov.L   r2, r5
 115 001f A8 8A                         mov.L   12[r0], r2
 120 0021 FC 43 41                      xchg    r4, r1
 122 0024 FD BF 56                      shar    #31, r5, r6
 125 0027 7F 8C                         RMPA.B
 134 0029 EF 41                         mov.L   r4, r1
 135 002b EF 52                         mov.L   r5, r2
 136 002d 3F 66 02                      rtsd    #8, r6-r6
*/
/* GNURX 2018q1 -O0, (This caller is not good example, to be reconsidered...)
 185 00ab 6E 6C                         pushm   r6-r12
 187 00ad 71 06 D8                      add     #-40, r0, r6
 189 00b0 EF 60                         mov.L   r6, r0
 191 00b2 75 45 48                      mov.L   #0x48, r5
 192 00b5 4B 65                         add     r6, r5
 193 00b7 A1 69                         mov.L   r1, 20[r6]
 194 00b9 A1 E2                         mov.L   r2, 24[r6]
 195 00bb A1 EB                         mov.L   r3, 28[r6]
 196 00bd A2 64                         mov.L   r4, 32[r6]
 197 00bf A9 6C                         mov.L   20[r6], r4
 198 00c1 E3 64                         mov.L   r4, [r6]
 199 00c3 A9 E4                         mov.L   24[r6], r4
 200 00c5 A0 6C                         mov.L   r4, 4[r6]
 201 00c7 A9 EC                         mov.L   28[r6], r4
 202 00c9 A0 E4                         mov.L   r4, 8[r6]
 203 00cb AA 64                         mov.L   32[r6], r4
 204 00cd A0 EC                         mov.L   r4, 12[r6]
 205 00cf EC 55                         mov.L   [r5], r5
 206 00d1 A1 65                         mov.L   r5, 16[r6]
 210 00d3 EC 64                         mov.L   [r6], r4
 211 00d5 A8 6D                         mov.L   4[r6], r5
 213 00d7 A8 E9                         mov.L   12[r6], r1
 215 00d9 A9 62                         mov.L   16[r6], r2
 217 00db A8 E3                         mov.L   8[r6], r3
 219 00dd ED 6C 01                      mov.L   4[r6], r12
 220 00e0 FD BF CA                      shar    #31, r12, r10
 221 00e3 ED 6C 01                      mov.L   4[r6], r12
 222 00e6 FD BF CB                      shar    #31, r12, r11
 223 00e9 EF A8                         mov.L   r10, r8
 225 00eb ED 6A 03                      mov.L   12[r6], r10
 226 00ee ED 6B 04                      mov.L   16[r6], r11
 228 00f1 FC 43 68                      XCHG R6, R8
 229 00f4 7F 8C                         RMPA.B
 230 00f6 FC 43 68                      XCHG R6, R8
 239 00f9 EF 42                         mov.L   r4, r2
 240 00fb EF 53                         mov.L   r5, r3
 241 00fd EF 24                         mov.L   r2, r4
 242 00ff EF 35                         mov.L   r3, r5
 246 0101 EF 42                         mov.L   r4, r2
 247 0103 EF 53                         mov.L   r5, r3
 248 0105 EF 24                         mov.L   r2, r4
 249 0107 EF 35                         mov.L   r3, r5
 250 0109 EF 41                         mov.L   r4, r1
 251 010b EF 52                         mov.L   r5, r2
 252 010d 3F 6C 11                      rtsd    #68, r6-r12
*/
}

#ifndef rmpaw
#define rmpaw(init, count, addr1, addr2) ccrx_machine_h_rmpaw(init, count, addr1, addr2)
#endif
__STATIC_FORCEINLINE long long ccrx_machine_h_rmpaw(long long init, unsigned long count, short *addr1, short *addr2)
{
/* CC-RX V2.03 -optimize=2
        ARG init = R2:R1
        ARG count = R3
        ARG addr1 = R4
        ARG addr2 = 04H[R0](08H[R0] after PUSH R6)
        RET R2:R1

        PUSH.L R6
        MOV.L R1, R14
        MOV.L R2, R5
        MOV.L 08H[R0], R2
        SHAR #1FH, R5, R6
        MOV.L R4, R1
        MOV.L R14, R4
        RMPA.W
        MOV.L R5, R2
        MOV.L R4, R1
        RTSD #04H, R6-R6
*/
    register long long _r5r4 __ASM("r4") = init;
    register short *_r1 __ASM("r1") = addr1;
    register short *_r2 __ASM("r2") = addr2;
    register unsigned long _r3 __ASM("r3") = count;
#ifdef __OPTIMIZE__
    /* In case of other than -O0, we assign r6. */
    register signed long _rS __ASM("r6") = (signed long)(init >> 63);
#else
    /* In case of -O0, we assign r8 because r6 and r7 are reserved for the frame pointer and arg(?) pointer. */
    register signed long _rS __ASM("r8") = (signed long)(init >> 63);
#endif
    __ASM volatile
    (
        /* AssemblerTemplate */
#ifdef __OPTIMIZE__
        "RMPA.W"                        "\n\t"
#else
        "XCHG R6, R8"                   "\n\t"
        "RMPA.W"                        "\n\t"
        "XCHG R6, R8"                   "\n\t"
#endif
        : /* OutputOperands */
            /**/ "+r" (_rS),
            /**/ "+r" (_r5r4),
            /**/ "+r" (_r3)
        : /* InputOperands */
            /**/ "r" (_r1),
            /**/ "r" (_r2),
            /**/ "m" (*(signed short (*)[count]) addr1),
            /**/ "m" (*(signed short (*)[count]) addr2)
        : /* Clobbers */
            /*"r1",*/ /* This causes a compile error: asm-specifier for variable '_r1' conflicts with asm clobber list */
            /*"r2" */ /* This causes a compile error: asm-specifier for variable '_r2' conflicts with asm clobber list */
    );
    __ASM volatile
    (
        /* AssemblerTemplate */
        ""/*"NOP"*/                     "\n\t"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            /* No inputs. */
        : /* Clobbers */
            "r1",
            "r2"
        /* This asm statement can make the compiler to recognize that r1 and r2 are not equivalent to addr1 and addr2. */
        /* Please see the comment in the rmpab() function. */
    );
    return _r5r4;
/* GNURX 2018q1 -O2
The difference from the rmpab() is the RMPA.B --> RMPA.W instruction.
*/
}

#ifndef rmpal
#define rmpal(init, count, addr1, addr2) ccrx_machine_h_rmpal(init, count, addr1, addr2)
#endif
__STATIC_FORCEINLINE long long ccrx_machine_h_rmpal(long long init, unsigned long count, long *addr1, long *addr2)
{
/* CC-RX V2.03 -optimize=2
        ARG init = R2:R1
        ARG count = R3
        ARG addr1 = R4
        ARG addr2 = 04H[R0](08H[R0] after PUSH R6)
        RET R2:R1

        PUSH.L R6
        MOV.L R1, R14
        MOV.L R2, R5
        MOV.L 08H[R0], R2
        SHAR #1FH, R5, R6
        MOV.L R4, R1
        MOV.L R14, R4
        RMPA.L
        MOV.L R5, R2
        MOV.L R4, R1
        RTSD #04H, R6-R6
*/
    register long long _r5r4 __ASM("r4") = init;
    register long *_r1 __ASM("r1") = addr1;
    register long *_r2 __ASM("r2") = addr2;
    register unsigned long _r3 __ASM("r3") = count;
#ifdef __OPTIMIZE__
    /* In case of other than -O0, we assign r6. */
    register signed long _rS __ASM("r6") = (signed long)(init >> 63);
#else
    /* In case of -O0, we assign r8 because r6 and r7 are reserved for the frame pointer and arg(?) pointer. */
    register signed long _rS __ASM("r8") = (signed long)(init >> 63);
#endif
    __ASM volatile
    (
        /* AssemblerTemplate */
#ifdef __OPTIMIZE__
        "RMPA.L"                        "\n\t"
#else
        "XCHG R6, R8"                   "\n\t"
        "RMPA.L"                        "\n\t"
        "XCHG R6, R8"                   "\n\t"
#endif
        : /* OutputOperands */
            /**/ "+r" (_rS),
            /**/ "+r" (_r5r4),
            /**/ "+r" (_r3)
        : /* InputOperands */
            /**/ "r" (_r1),
            /**/ "r" (_r2),
            /**/ "m" (*(signed long (*)[count]) addr1),
            /**/ "m" (*(signed long (*)[count]) addr2)
        : /* Clobbers */
            /*"r1",*/ /* This causes a compile error: asm-specifier for variable '_r1' conflicts with asm clobber list */
            /*"r2" */ /* This causes a compile error: asm-specifier for variable '_r2' conflicts with asm clobber list */
    );
    __ASM volatile
    (
        /* AssemblerTemplate */
        ""/*"NOP"*/                     "\n\t"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            /* No inputs. */
        : /* Clobbers */
            "r1",
            "r2"
        /* This asm statement can make the compiler to recognize that r1 and r2 are not equivalent to addr1 and addr2. */
        /* Please see the comment in the rmpab() function. */
    );
    return _r5r4;
/* GNURX 2018q1 -O2
The difference from the rmpab() is the RMPA.B --> RMPA.L instruction.
*/
}

#ifndef rolc
#define rolc(data) ccrx_machine_h_rolc(data)
#endif
__STATIC_FORCEINLINE long ccrx_machine_h_rolc(unsigned long data)
{
/* CC-RX V2.03 -optimize=2
        ROLC R1
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "ROLC %[R1]"
        : /* OutputOperands */
            [R1] "+r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 235 0067 7E 51                         ROLC r1
 241 0069 02                            rts
*/
}

#ifndef rorc
#define rorc(data) ccrx_machine_h_rorc(data)
#endif
__STATIC_FORCEINLINE long ccrx_machine_h_rorc(unsigned long data)
{
/* CC-RX V2.03 -optimize=2
        RORC R1
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "RORC %[R1]"
        : /* OutputOperands */
            [R1] "+r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 254 006a 7E 41                         RORC r1
 260 006c 02                            rts
*/
}

#ifndef rotl
#define rotl(data, num) ccrx_machine_h_rotl(data, num)
#endif
__STATIC_FORCEINLINE long ccrx_machine_h_rotl(unsigned long data, unsigned long num)
{
/* CC-RX V2.03 -optimize=2
        ROTL R2, R1
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "ROTL %[R2], %[R1]"
        : /* OutputOperands */
            [R1] "+r" (data)
        : /* InputOperands */
            [R2] "r" (num)
    );
    return data;
/* GNURX 2018q1 -O2
 292 0071 FD 64 21                      ROTR r2, r1
 298 0074 02                            rts
*/
}

#ifndef rotr
#define rotr(data, num) ccrx_machine_h_rotr(data, num)
#endif
__STATIC_FORCEINLINE long ccrx_machine_h_rotr(unsigned long data, unsigned long num)
{
/* CC-RX V2.03 -optimize=2
        ROTR R2, R1
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "ROTR %[R2], %[R1]"
        : /* OutputOperands */
            [R1] "+r" (data)
        : /* InputOperands */
            [R2] "r" (num)
    );
    return data;
/* GNURX 2018q1 -O2
 292 0071 FD 64 21                      ROTR r2, r1
 298 0074 02                            rts
*/
}

#ifndef brk
#define brk() ccrx_machine_h_brk()
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_brk(void)
{
/* CC-RX V2.03 -optimize=2
        BRK
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "BRK"
        : /* OutputOperands */
            /* No outputs. */
    );
    return;
/* GNURX 2018q1 -O2
 310 0075 00                            BRK
 312 0076 02                            rts
*/
}

#ifndef int_exception
#define int_exception(num) ccrx_machine_h_int_exception(num)
#endif
#ifdef DONT_EXECUTE_RX_INT_INSTRUCTION_ON_STACK

void ccrx_machine_h_int_exception_switch_case_function(signed long num);

#define ccrx_machine_h_int_exception(num) \
do { \
    if (__builtin_constant_p(num)) { \
        __builtin_rx_int(num); \
    } else { \
        ccrx_machine_h_int_exception_switch_case_function(num); \
    } \
} while (0)

#else /* DONT_EXECUTE_RX_INT_INSTRUCTION_ON_STACK */

#define ccrx_machine_h_int_exception(num) \
do { \
    if (__builtin_constant_p(num)) { \
        __builtin_rx_int(num); \
    } else { \
        unsigned char int_rts[] = {0x75, 0x60, num, 0x02}; \
        ((void (*)(void*))int_rts)(int_rts); \
    } \
} while (0)

#endif /* DONT_EXECUTE_RX_INT_INSTRUCTION_ON_STACK */

#ifndef wait
#define wait() ccrx_machine_h_wait()
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_wait(void)
{
/* CC-RX V2.03 -optimize=2
        WAIT
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "WAIT"
        : /* OutputOperands */
            /* No outputs. */
    );
    return;
/* GNURX 2018q1 -O2
 372 0092 7F 96                         WAIT
 374 0094 02                            rts
*/
}

#ifndef nop
#define nop() ccrx_machine_h_nop()
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_nop(void)
{
/* CC-RX V2.03 -optimize=2
        NOP
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "NOP"
        : /* OutputOperands */
            /* No outputs. */
    );
    return;
/* GNURX 2018q1 -O2
 388 0095 03                            NOP
 390 0096 02                            rts
*/
}

#ifndef set_ipl
#define set_ipl(level) ccrx_machine_h_set_ipl(level)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_ipl(signed long level)
{
/* CC-RX V2.03 -optimize=2
        MVFC PSW, R14
        SHLL #1CH, R1
        SHLR #04H, R1
        AND #0F0FFFFFFH, R14
        OR R14, R1
        MVTC R1, PSW
        RTS
*/
    register unsigned long temp;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC PSW, %[R14]"              "\n\t"
        "SHLL #0x1C, %[R1]"             "\n\t"
        "SHLR #0x04, %[R1]"             "\n\t"
        "AND #0xF0FFFFFF, %[R14]"       "\n\t"
        "OR %[R14], %[R1]"              "\n\t"
        "MVTC %[R1], PSW"               "\n\t"
        : /* OutputOperands */
            [R14] "=&r" (temp),
            [R1] "+r" (level)
    );
    return;
/* GNURX 2018q1 -O2
 377 008e FD 6A 05                      MVFC PSW, r5
 378 0091 6D C1                         SHLL #0x1C, r1
 379 0093 68 41                         SHLR #0x04, r1
 380 0095 74 25 FF FF FF F0             AND #0xF0FFFFFF, r5
 381 009b 57 51                         OR r5, r1
 382 009d FD 68 10                      MVTC r1, PSW
 385 00a0 02                            rts
*/
}

#ifndef get_ipl
#define get_ipl() ccrx_machine_h_get_ipl()
#endif
__STATIC_FORCEINLINE unsigned char ccrx_machine_h_get_ipl(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC PSW, R14
        REVL R14, R1
        AND #0FH, R1
        RTS
*/
    register unsigned long temp;
    register unsigned char level;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC PSW, %[R14]"              "\n\t"
        "REVL %[R14], %[R1]"            "\n\t"
        "AND #0x0F, %[R1]"              "\n\t"
        : /* OutputOperands */
            [R14] "=r" (temp),
            [R1] "=r" (level)
    );
    return level;
/* GNURX 2018q1 -O2
 399 00a1 FD 6A 05                      MVFC PSW, r5
 400 00a4 FD 67 51                      REVL r5, r1
 401 00a7 64 F1                         AND #0x0F, r1
 407 00a9 5B 11                         movu.B  r1, r1      ★ TODO: check
 408 00ab 02                            rts
*/
}

#ifndef set_psw
#define set_psw(data) ccrx_machine_h_set_psw(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_psw(unsigned long data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, PSW
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], PSW"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 449 00b5 FD 68 10                      MVTC r1, PSW
 451 00b8 02                            rts
*/
}

#ifndef get_psw
#define get_psw() ccrx_machine_h_get_psw()
#endif
__STATIC_FORCEINLINE unsigned long ccrx_machine_h_get_psw(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC PSW, R1
        RTS
*/
    register unsigned long data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC PSW, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 465 00b9 FD 6A 01                      MVFC PSW, r1
 471 00bc 02                            rts
*/
}

#ifndef set_fpsw
#define set_fpsw(data) ccrx_machine_h_set_fpsw(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_fpsw(unsigned long data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, FPSW
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], FPSW"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 484 00bd FD 68 13                      MVTC r1, FPSW
 486 00c0 02                            rts
*/
}

#ifndef get_fpsw
#define get_fpsw() ccrx_machine_h_get_fpsw()
#endif
__STATIC_FORCEINLINE unsigned long ccrx_machine_h_get_fpsw(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC FPSW, R1
        RTS
*/
    register unsigned long data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC FPSW, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 500 00c1 FD 6A 31                      MVFC FPSW, r1
 506 00c4 02                            rts
*/
}

#ifndef set_usp
#define set_usp(data) ccrx_machine_h_set_usp(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_usp(void *data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, USP
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], USP"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 519 00c5 FD 68 12                      MVTC r1, USP
 521 00c8 02                            rts
*/
}

#ifndef get_usp
#define get_usp() ccrx_machine_h_get_usp()
#endif
__STATIC_FORCEINLINE void *ccrx_machine_h_get_usp(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC USP, R1
        RTS
*/
    register void *data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC USP, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 535 00c9 FD 6A 21                      MVFC USP, r1
 541 00cc 02                            rts
*/
}

#ifndef set_isp
#define set_isp(data) ccrx_machine_h_set_isp(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_isp(void *data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, ISP
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], ISP"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 554 00cd FD 68 1A                      MVTC r1, ISP
 556 00d0 02                            rts
*/
}

#ifndef get_isp
#define get_isp() ccrx_machine_h_get_isp()
#endif
__STATIC_FORCEINLINE void *ccrx_machine_h_get_isp(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC ISP, R1
        RTS
*/
    register void *data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC ISP, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 570 00d1 FD 6A A1                      MVFC ISP, r1
 576 00d4 02                            rts
*/
}

#ifndef set_intb
#define set_intb(data) ccrx_machine_h_set_intb(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_intb(void *data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, INTB
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], INTB"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 589 00d5 FD 68 1C                      MVTC r1, INTB
 591 00d8 02                            rts
*/
}

#ifndef get_intb
#define get_intb() ccrx_machine_h_get_intb()
#endif
__STATIC_FORCEINLINE void *ccrx_machine_h_get_intb(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC INTB, R1
        RTS
*/
    register void *data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC INTB, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 605 00d9 FD 6A C1                      MVFC INTB, r1
 611 00dc 02                            rts
*/
}

#ifndef set_bpsw
#define set_bpsw(data) ccrx_machine_h_set_bpsw(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_bpsw(unsigned long data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, BPSW
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], BPSW"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 624 00dd FD 68 18                      MVTC r1, BPSW
 626 00e0 02                            rts
*/
}

#ifndef get_bpsw
#define get_bpsw() ccrx_machine_h_get_bpsw()
#endif
__STATIC_FORCEINLINE unsigned long ccrx_machine_h_get_bpsw(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC BPSW, R1
        RTS
*/
    register unsigned long data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC BPSW, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 640 00e1 FD 6A 81                      MVFC BPSW, r1
 646 00e4 02                            rts
*/
}

#ifndef set_bpc
#define set_bpc(data) ccrx_machine_h_set_bpc(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_bpc(void *data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, BPC
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], BPC"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 659 00e5 FD 68 19                      MVTC r1, BPC
 661 00e8 02                            rts
*/
}

#ifndef get_bpc
#define get_bpc() ccrx_machine_h_get_bpc()
#endif
__STATIC_FORCEINLINE void *ccrx_machine_h_get_bpc(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC BPC, R1
        RTS
*/
    register void *data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC BPC, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 675 00e9 FD 6A 91                      MVFC BPC, r1
 681 00ec 02                            rts
*/
}

#ifndef set_fintv
#define set_fintv(data) ccrx_machine_h_set_fintv(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_fintv(void *data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, FINTV
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], FINTV"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 694 00ed FD 68 1B                      MVTC r1, FINTV
 696 00f0 02                            rts
*/
}

#ifndef get_fintv
#define get_fintv() ccrx_machine_h_get_fintv()
#endif
__STATIC_FORCEINLINE void *ccrx_machine_h_get_fintv(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC FINTV, R1
        RTS
*/
    register void *data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC FINTV, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 710 00f1 FD 6A B1                      MVFC FINTV, r1
 716 00f4 02                            rts
*/
}

#ifndef emul
#define emul(data1, data2) ccrx_machine_h_emul(data1, data2)
#endif
__STATIC_FORCEINLINE signed long long ccrx_machine_h_emul(signed long data1, signed long data2)
{
/* CC-RX V2.03 -optimize=2
        MOV.L R1, R4
        EMUL R2, R4
        MOV.L R5, R2
        MOV.L R4, R1
        RTS
*/
    return (signed long long)data1 * data2;
/* GNURX 2018q1 -O2
 728 00f5 FC 1B 12                      emul    r1, r2
 733 00f8 EF 21                         mov.L   r2, r1
 735 00fa EF 32                         mov.L   r3, r2
 736 00fc 02                            rts
*/
}

#ifndef emulu
#define emulu(data1, data2) ccrx_machine_h_emulu(data1, data2)
#endif
__STATIC_FORCEINLINE unsigned long long ccrx_machine_h_emulu(unsigned long data1, unsigned long data2)
{
/* CC-RX V2.03 -optimize=2
        MOV.L R1, R4
        EMULU R2, R4
        MOV.L R5, R2
        MOV.L R4, R1
        RTS
*/
    return (unsigned long long)data1 * data2;
/* GNURX 2018q1 -O2
 748 00fd FC 1F 12                      emulu   r1, r2
 753 0100 EF 21                         mov.L   r2, r1
 755 0102 EF 32                         mov.L   r3, r2
 756 0104 02                            rts
*/
}

#ifndef chg_pmusr
#define chg_pmusr() ccrx_machine_h_chg_pmusr()
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_chg_pmusr(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC PSW, R14
        BTST #14H, R14
        BNE L48
L47:    ; entry
        OR #00100000H, R14
        PUSH.L R14
L49:
        MVFC PC, R14
        ADD #L48-L49, R14
        PUSH.L R14
        RTE
L48:    ; entry
        RTS
*/
   register unsigned long temp;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC PSW, %[R14]"              "\n\t"
        "BTST #0x14, %[R14]"            "\n\t"
        "BNE 3f"                        "\n"
"1:"                                    "\n\t"
        "OR #0x00100000, %[R14]"        "\n\t"
        "PUSH.L %[R14]"                 "\n"
"2:"                                    "\n\t"
        "MVFC PC, %[R14]"               "\n\t"
        "ADD #3f-2b, %[R14]"            "\n\t"
        "PUSH.L %[R14]"                 "\n\t"
        "RTE"                           "\n"
"3:"                                    "\n\t"
        : /* OutputOperands */
            [R14] "=r" (temp)
    );
    return;
/* GNURX 2018q1 -O2
 740 00fc FD 6A 05                      MVFC PSW, r5
 741 00ff 7D 45                         BTST #0x14, r5
 742 0101 21 13                         BNE 3f
 743                                1:
 744 0103 77 35 00 00 10                OR #0x00100000, r5
 745 0108 7E A5                         PUSH.L r5
 746                                2:
 747 010a FD 6A 15                      MVFC PC, r5
 748 010d 71 55 0A                      ADD #3f-2b, r5
 749 0110 7E A5                         PUSH.L r5
 750 0112 7F 95                         RTE
 751                                3:
 754 0114 02                            rts
*/
}

#ifndef set_acc
#define set_acc(data) ccrx_machine_h_set_acc(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_acc(signed long long data)
{
/* CC-RX V2.03 -optimize=2
        MVTACLO R1
        MVTACHI R2
        RTS
*/
#if 1
    register union llreg_u {
        unsigned long w32[2];
        signed long long w64;
    } temp;
    temp.w64 = data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTACLO %[R1]"                 "\n\t"
        "MVTACHI %[R2]"                 "\n\t"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (temp.w32[0]),
            [R2] "r" (temp.w32[1])
    );
    return;
/* GNURX 2018q1 -O2 -mlittle-endian-data
 797 0115 FD 17 11                      MVTACLO r1
 798 0118 FD 17 02                      MVTACHI r2
 801 011b 02                            rts
*/
/* GNURX 2018q1 -O2 -mbig-endian-data
 830 0134 FD 17 11                      MVTACLO r1
 831 0137 FD 17 02                      MVTACHI r2
 834 013a 02                            rts
*/
/* GNURX 2018q1 -O0 -mlittle-endian-data
 1303 04d0 6E 6B                        pushm   r6-r11
 1305 04d2 71 06 F0                     add 	#-16, r0, r6
 1307 04d5 EF 60                        mov.L   r6, r0
 1309 04d7 A0 E1                        mov.L   r1, 8[r6]
 1310 04d9 A0 EA                        mov.L   r2, 12[r6]
 1311 04db A8 E5                        mov.L   8[r6], r5
 1312 04dd E3 65                        mov.L   r5, [r6]
 1313 04df A8 ED                        mov.L   12[r6], r5
 1314 04e1 A0 6D                        mov.L   r5, 4[r6]
 1318 04e3 EC 6A                        mov.L   [r6], r10
 1319 04e5 ED 6B 01                     mov.L   4[r6], r11
 1321 04e8 EF A5                        mov.L   r10, r5
 1323 04ea EF B4                        mov.L   r11, r4
 1326 04ec FD 17 15                     MVTACLO r5
 1327 04ef FD 17 04                     MVTACHI r4
 1333 04f2 3F 6B 0A                     rtsd    #40, r6-r11
*/
/* GNURX 2018q1 -O0 -mbig-endian-data
 1313 04da 6E 6B                        pushm   r6-r11
 1315 04dc 71 06 F0                     add     #-16, r0, r6
 1317 04df EF 60                        mov.L   r6, r0
 1319 04e1 A0 E1                        mov.L   r1, 8[r6]
 1320 04e3 A0 EA                        mov.L   r2, 12[r6]
 1321 04e5 A8 E5                        mov.L   8[r6], r5
 1322 04e7 E3 65                        mov.L   r5, [r6]
 1323 04e9 A8 ED                        mov.L   12[r6], r5
 1324 04eb A0 6D                        mov.L   r5, 4[r6]
 1328 04ed EC 6A                        mov.L   [r6], r10
 1329 04ef ED 6B 01                     mov.L   4[r6], r11
 1331 04f2 EF A5                        mov.L   r10, r5
 1333 04f4 EF B4                        mov.L   r11, r4
 1336 04f6 FD 17 15                     MVTACLO r5
 1337 04f9 FD 17 04                     MVTACHI r4
 1343 04fc 3F 6B 0A                     rtsd    #40, r6-r11
*/
#elif 0
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTACLO %L[R99]"               "\n\t"
        "MVTACHI %H[R99]"               "\n\t"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R99] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 791 0115 EF 14                         mov.L   r1, r4
 792 0117 EF 25                         mov.L   r2, r5
 798 0119 FD 17 14                      MVTACLO r4
 799 011c FD 17 05                      MVTACHI r5
 801 011f 02                            rts
*/
#endif
}

#ifndef get_acc
#define get_acc() ccrx_machine_h_get_acc()
#endif
__STATIC_FORCEINLINE signed long long ccrx_machine_h_get_acc(void)
{
/* CC-RX V2.03 -optimize=2
        MVFACMI R1
        SHLL #10H, R1
        MVFACHI R2
        RTS
*/
#if 1
    register union llreg_u {
        unsigned long w32[2];
        signed long long w64;
    } result;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFACMI %[R1]"                 "\n\t"
        "SHLL #0x10, %[R1]"             "\n\t"
        "MVFACHI %[R2]"                 "\n\t"
        : /* OutputOperands */
            [R1] "=r" (result.w32[0]),
            [R2] "=r" (result.w32[1])
    );
    return result.w64;
/* GNURX 2018q1 -O2 -mlittle-endian-data
 813 011c FD 1F 21                      MVFACMI r1
 814 011f 6D 01                         SHLL #0x10, r1
 815 0121 FD 1F 02                      MVFACHI r2
 818 0124 02                            rts
*/
/* GNURX 2018q1 -O2 -mbig-endian-data
 846 013b FD 1F 21                      MVFACMI r1
 847 013e 6D 01                         SHLL #0x10, r1
 848 0140 FD 1F 02                      MVFACHI r2
 851 0143 02                            rts
*/
/* GNURX 2018q1 -O0 -mlittle-endian-data
 1341 04f5 6E 6B                        pushm   r6-r11
 1343 04f7 EF 06                        mov.L   r0, r6
 1349 04f9 FD 1F 24                     MVFACMI r4
 1350 04fc 6D 04                        SHLL    #0x10, r4
 1351 04fe FD 1F 05                     MVFACHI r5
 1354 0501 EF 4A                        mov.L   r4, r10
 1355 0503 EF 5B                        mov.L   r5, r11
 1357 0505 EF A2                        mov.L   r10, r2
 1358 0507 EF B3                        mov.L   r11, r3
 1359 0509 EF 24                        mov.L   r2, r4
 1360 050b EF 35                        mov.L   r3, r5
 1364 050d EF 42                        mov.L   r4, r2
 1365 050f EF 53                        mov.L   r5, r3
 1366 0511 EF 24                        mov.L   r2, r4
 1367 0513 EF 35                        mov.L   r3, r5
 1368 0515 EF 41                        mov.L   r4, r1
 1369 0517 EF 52                        mov.L   r5, r2
 1370 0519 3F 6B 06                     rtsd    #24, r6-r11
*/
/* GNURX 2018q1 -O2 -mbig-endian-data
 1351 04ff 6E 6B                        pushm   r6-r11
 1353 0501 EF 06                        mov.L   r0, r6
 1359 0503 FD 1F 24                     MVFACMI r4
 1360 0506 6D 04                        SHLL    #0x10, r4
 1361 0508 FD 1F 05                     MVFACHI r5
 1364 050b EF 4A                        mov.L   r4, r10
 1365 050d EF 5B                        mov.L   r5, r11
 1367 050f EF A2                        mov.L   r10, r2
 1368 0511 EF B3                        mov.L   r11, r3
 1369 0513 EF 24                        mov.L   r2, r4
 1370 0515 EF 35                        mov.L   r3, r5
 1374 0517 EF 42                        mov.L   r4, r2
 1375 0519 EF 53                        mov.L   r5, r3
 1376 051b EF 24                        mov.L   r2, r4
 1377 051d EF 35                        mov.L   r3, r5
 1378 051f EF 41                        mov.L   r4, r1
 1379 0521 EF 52                        mov.L   r5, r2
 1380 0523 3F 6B 06                     rtsd    #24, r6-r11
*/
#elif 0
    register signed long long data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFACMI %L[R99]"               "\n\t"
        "SHLL #0x10, %L[R99]"           "\n\t"
        "MVFACHI %H[R99]"               "\n\t"
        : /* OutputOperands */
            [R99] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 811 011c FD 1F 24                      MVFACMI r4
 812 011f 6D 04                         SHLL #0x10, r4
 813 0121 FD 1F 05                      MVFACHI r5
 819 0124 EF 41                         mov.L   r4, r1
 820 0126 EF 52                         mov.L   r5, r2
 821 0128 02                            rts
*/
#endif
}

#ifndef setpsw_i
#define setpsw_i() ccrx_machine_h_setpsw_i()
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_setpsw_i(void)
{
/* CC-RX V2.03 -optimize=2
        SETPSW I
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "SETPSW I"
        : /* OutputOperands */
            /* No outputs. */
    );
    return;
/* GNURX 2018q1 -O2
 829 012e 7F A8                         SETPSW I
 831 0130 02                            rts
*/
}

#ifndef clrpsw_i
#define clrpsw_i() ccrx_machine_h_clrpsw_i()
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_clrpsw_i(void)
{
/* CC-RX V2.03 -optimize=2
        CLRPSW I
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "CLRPSW I"
        : /* OutputOperands */
            /* No outputs. */
    );
    return;
/* GNURX 2018q1 -O2
 845 0131 7F B8                         CLRPSW I
 847 0133 02                            rts
*/
}

#ifndef macl
#define macl(data1, data2, count) ccrx_machine_h_macl(data1, data2, count)
#endif
__STATIC_FORCEINLINE long ccrx_machine_h_macl(short *data1, short *data2, unsigned long count)
{
/* CC-RX V2.03 -optimize=2
        CMP #00H, R3
        MOV.L #00000000H, R14
        MULLO R14, R14
        BEQ L60
L55:    ; entry
        BTST #00H, R3
        BGEU L57
L56:    ; entry
        SUB #02H, R3
        MOV.L [R1+], R14
        MOV.L [R2+], R5
        MACLO R14, R5
        MACHI R14, R5
        BNE L56
        BRA L60
L57:    ; entry
        SUB #01H, R3
        BEQ L59
L58:    ; entry
        SUB #02H, R3
        MOV.L [R1+], R14
        MOV.L [R2+], R5
        MACLO R14, R5
        MACHI R14, R5
        BNE L58
L59:    ; entry
        MOV.W [R1], R14
        MOV.W [R2], R5
        MACLO R14, R5
L60:    ; entry
        MVFACMI R1
        RTS
*/
    register long temp1;
    register long temp2;
    register long result;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "CMP #0x00, %[R3]"              "\n\t"
        "MOV.L #0x00000000, %[R14]"     "\n\t"
        "MULLO %[R14], %[R14]"          "\n\t"
        "BEQ 6f"                        "\n"  
"1:"                                    "\n\t"
        "BTST #0x00, %[R3]"             "\n\t"
        "BGEU 3f"                       "\n"  
"2:"                                    "\n\t"
        "SUB #0x02, %[R3]"              "\n\t"
        "MOV.L [%[R1]+], %[R14]"        "\n\t"
        "MOV.L [%[R2]+], %[R5]"         "\n\t"
        "MACLO %[R14], %[R5]"           "\n\t"
        "MACHI %[R14], %[R5]"           "\n\t"
        "BNE 2b"                        "\n\t"
        "BRA 6f"                        "\n"  
"3:"                                    "\n\t"
        "SUB #0x01, %[R3]"              "\n\t"
        "BEQ 5f"                        "\n"  
"4:"                                    "\n\t"
        "SUB #0x02, %[R3]"              "\n\t"
        "MOV.L [%[R1]+], %[R14]"        "\n\t"
        "MOV.L [%[R2]+], %[R5]"         "\n\t"
        "MACLO %[R14], %[R5]"           "\n\t"
        "MACHI %[R14], %[R5]"           "\n\t"
        "BNE 4b"                        "\n"  
"5:"                                    "\n\t"
        "MOV.W [%[R1]], %[R14]"         "\n\t"
        "MOV.W [%[R2]], %[R5]"          "\n\t"
        "MACLO %[R14], %[R5]"           "\n"  
"6:"                                    "\n\t"
        : /* OutputOperands */
            [R1] "+r" (data1),
            [R2] "+r" (data2),
            [R3] "+r" (count),
            [R5] "=&r" (temp1),
            [R14] "=&r" (temp2)
        : /* InputOperands */
            /**/ "m" (*(short (*)[count]) data1),
            /**/ "m" (*(short (*)[count]) data2)
    );
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFACMI %[R1]"                 "\n\t"
        : /* OutputOperands */
            [R1] "=r" (result)
    );
    return result;
/* GNURX 2018q1 -O2
 834 012b 61 03                         CMP #0x00, r3
 835 012d 66 04                         MOV.L #0x00000000, r4
 836 012f FD 01 44                      MULLO r4, r4
 837 0132 20 33                         BEQ 6f
 838                                    1:
 839 0134 7C 03                         BTST #0x00, r3
 840 0136 22 14                         BGEU 3f
 841                                    2:
 842 0138 60 23                         SUB #0x02, r3
 843 013a FD 2A 14                      MOV.L [r1+], r4
 844 013d FD 2A 25                      MOV.L [r2+], r5
 845 0140 FD 05 45                      MACLO r4, r5
 846 0143 FD 04 45                      MACHI r4, r5
 847 0146 21 F2                         BNE 2b
 848 0148 2E 1D                         BRA 6f
 849                                    3:
 850 014a 60 13                         SUB #0x01, r3
 851 014c 20 12                         BEQ 5f
 852                                    4:
 853 014e 60 23                         SUB #0x02, r3
 854 0150 FD 2A 14                      MOV.L [r1+], r4
 855 0153 FD 2A 25                      MOV.L [r2+], r5
 856 0156 FD 05 45                      MACLO r4, r5
 857 0159 FD 04 45                      MACHI r4, r5
 858 015c 21 F2                         BNE 4b
 859                                    5:
 860 015e DC 14                         MOV.W [r1], r4
 861 0160 DC 25                         MOV.W [r2], r5
 862 0162 FD 05 45                      MACLO r4, r5
 868 0165 FD 1F 21                      MVFACMI r1
 874 0168 02                            rts
*/
}

#ifndef macw1
#define macw1(data1, data2, count) ccrx_machine_h_macw1(data1, data2, count)
#endif
__STATIC_FORCEINLINE short ccrx_machine_h_macw1(short *data1, short *data2, unsigned long count)
{
/* CC-RX V2.03 -optimize=2
        CMP #00H, R3
        MOV.L #00000000H, R14
        MULLO R14, R14
        BEQ L67
L62:    ; entry
        BTST #00H, R3
        BGEU L64
L63:    ; entry
        SUB #02H, R3
        MOV.L [R1+], R14
        MOV.L [R2+], R5
        MACLO R14, R5
        MACHI R14, R5
        BNE L63
        BRA L67
L64:    ; entry
        SUB #01H, R3
        BEQ L66
L65:    ; entry
        SUB #02H, R3
        MOV.L [R1+], R14
        MOV.L [R2+], R5
        MACLO R14, R5
        MACHI R14, R5
        BNE L65
L66:    ; entry
        MOV.W [R1], R14
        MOV.W [R2], R5
        MACLO R14, R5
L67:    ; entry
        RACW #01H
        MVFACHI R1
        RTS
*/
    register long temp1;
    register long temp2;
    register short result;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "CMP #0x00, %[R3]"              "\n\t"
        "MOV.L #0x00000000, %[R14]"     "\n\t"
        "MULLO %[R14], %[R14]"          "\n\t"
        "BEQ 6f"                        "\n"  
"1:"                                    "\n\t"
        "BTST #0x00, %[R3]"             "\n\t"
        "BGEU 3f"                       "\n"  
"2:"                                    "\n\t"
        "SUB #0x02, %[R3]"              "\n\t"
        "MOV.L [%[R1]+], %[R14]"        "\n\t"
        "MOV.L [%[R2]+], %[R5]"         "\n\t"
        "MACLO %[R14], %[R5]"           "\n\t"
        "MACHI %[R14], %[R5]"           "\n\t"
        "BNE 2b"                        "\n\t"
        "BRA 6f"                        "\n"  
"3:"                                    "\n\t"
        "SUB #0x01, %[R3]"              "\n\t"
        "BEQ 5f"                        "\n"  
"4:"                                    "\n\t"
        "SUB #0x02, %[R3]"              "\n\t"
        "MOV.L [%[R1]+], %[R14]"        "\n\t"
        "MOV.L [%[R2]+], %[R5]"         "\n\t"
        "MACLO %[R14], %[R5]"           "\n\t"
        "MACHI %[R14], %[R5]"           "\n\t"
        "BNE 4b"                        "\n"  
"5:"                                    "\n\t"
        "MOV.W [%[R1]], %[R14]"         "\n\t"
        "MOV.W [%[R2]], %[R5]"          "\n\t"
        "MACLO %[R14], %[R5]"           "\n"  
"6:"                                    "\n\t"
        : /* OutputOperands */
            [R1] "+r" (data1),
            [R2] "+r" (data2),
            [R3] "+r" (count),
            [R5] "=&r" (temp1),
            [R14] "=&r" (temp2)
        : /* InputOperands */
            /**/ "m" (*(short (*)[count]) data1),
            /**/ "m" (*(short (*)[count]) data2)
    );
    __ASM volatile
    (
        /* AssemblerTemplate */
        "RACW #0x01"                    "\n\t"
        "MVFACMI %[R1]"                 "\n\t"
        : /* OutputOperands */
            [R1] "=r" (result)
    );
    return result;
/* GNURX 2018q1 -O2
 887 0169 61 03                         CMP #0x00, r3
 888 016b 66 04                         MOV.L #0x00000000, r4
 889 016d FD 01 44                      MULLO r4, r4
 890 0170 20 33                         BEQ 6f
 891                                    1:
 892 0172 7C 03                         BTST #0x00, r3
 893 0174 22 14                         BGEU 3f
 894                                    2:
 895 0176 60 23                         SUB #0x02, r3
 896 0178 FD 2A 14                      MOV.L [r1+], r4
 897 017b FD 2A 25                      MOV.L [r2+], r5
 898 017e FD 05 45                      MACLO r4, r5
 899 0181 FD 04 45                      MACHI r4, r5
 900 0184 21 F2                         BNE 2b
 901 0186 2E 1D                         BRA 6f
 902                                    3:
 903 0188 60 13                         SUB #0x01, r3
 904 018a 20 12                         BEQ 5f
 905                                    4:
 906 018c 60 23                         SUB #0x02, r3
 907 018e FD 2A 14                      MOV.L [r1+], r4
 908 0191 FD 2A 25                      MOV.L [r2+], r5
 909 0194 FD 05 45                      MACLO r4, r5
 910 0197 FD 04 45                      MACHI r4, r5
 911 019a 21 F2                         BNE 4b
 912                                    5:
 913 019c DC 14                         MOV.W [r1], r4
 914 019e DC 25                         MOV.W [r2], r5
 915 01a0 FD 05 45                      MACLO r4, r5
 916                                    6:
 921 01a3 FD 18 00                      RACW #0x01
 922 01a6 FD 1F 21                      MVFACMI r1
 928 01a9 DF 11                         mov.W   r1, r1      ★ TODO: check
 929 01ab 02                            rts
*/
}

#ifndef macw2
#define macw2(data1, data2, count) ccrx_machine_h_macw2(data1, data2, count)
#endif
__STATIC_FORCEINLINE short ccrx_machine_h_macw2(short *data1, short *data2, unsigned long count)
{
/* CC-RX V2.03 -optimize=2
        CMP #00H, R3
        MOV.L #00000000H, R14
        MULLO R14, R14
        BEQ L74
L69:    ; entry
        BTST #00H, R3
        BGEU L71
L70:    ; entry
        SUB #02H, R3
        MOV.L [R1+], R14
        MOV.L [R2+], R5
        MACLO R14, R5
        MACHI R14, R5
        BNE L70
        BRA L74
L71:    ; entry
        SUB #01H, R3
        BEQ L73
L72:    ; entry
        SUB #02H, R3
        MOV.L [R1+], R14
        MOV.L [R2+], R5
        MACLO R14, R5
        MACHI R14, R5
        BNE L72
L73:    ; entry
        MOV.W [R1], R14
        MOV.W [R2], R5
        MACLO R14, R5
L74:    ; entry
        RACW #02H
        MVFACHI R1
        RTS
*/
    register long temp1;
    register long temp2;
    register short result;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "CMP #0x00, %[R3]"              "\n\t"
        "MOV.L #0x00000000, %[R14]"     "\n\t"
        "MULLO %[R14], %[R14]"          "\n\t"
        "BEQ 6f"                        "\n"  
"1:"                                    "\n\t"
        "BTST #0x00, %[R3]"             "\n\t"
        "BGEU 3f"                       "\n"  
"2:"                                    "\n\t"
        "SUB #0x02, %[R3]"              "\n\t"
        "MOV.L [%[R1]+], %[R14]"        "\n\t"
        "MOV.L [%[R2]+], %[R5]"         "\n\t"
        "MACLO %[R14], %[R5]"           "\n\t"
        "MACHI %[R14], %[R5]"           "\n\t"
        "BNE 2b"                        "\n\t"
        "BRA 6f"                        "\n"  
"3:"                                    "\n\t"
        "SUB #0x01, %[R3]"              "\n\t"
        "BEQ 5f"                        "\n"  
"4:"                                    "\n\t"
        "SUB #0x02, %[R3]"              "\n\t"
        "MOV.L [%[R1]+], %[R14]"        "\n\t"
        "MOV.L [%[R2]+], %[R5]"         "\n\t"
        "MACLO %[R14], %[R5]"           "\n\t"
        "MACHI %[R14], %[R5]"           "\n\t"
        "BNE 4b"                        "\n"  
"5:"                                    "\n\t"
        "MOV.W [%[R1]], %[R14]"         "\n\t"
        "MOV.W [%[R2]], %[R5]"          "\n\t"
        "MACLO %[R14], %[R5]"           "\n"  
"6:"                                    "\n\t"
        : /* OutputOperands */
            [R1] "+r" (data1),
            [R2] "+r" (data2),
            [R3] "+r" (count),
            [R5] "=&r" (temp1),
            [R14] "=&r" (temp2)
        : /* InputOperands */
            /**/ "m" (*(short (*)[count]) data1),
            /**/ "m" (*(short (*)[count]) data2)
    );
    __ASM volatile
    (
        /* AssemblerTemplate */
        "RACW #02H"                     "\n\t"
        "MVFACMI %[R1]"                 "\n\t"
        : /* OutputOperands */
            [R1] "=r" (result)
    );
    return result;
/* GNURX 2018q1 -O2
 942 01ac 61 03                         CMP #0x00, r3
 943 01ae 66 04                         MOV.L #0x00000000, r4
 944 01b0 FD 01 44                      MULLO r4, r4
 945 01b3 20 33                         BEQ 6f
 946                                    1:
 947 01b5 7C 03                         BTST #0x00, r3
 948 01b7 22 14                         BGEU 3f
 949                                    2:
 950 01b9 60 23                         SUB #0x02, r3
 951 01bb FD 2A 14                      MOV.L [r1+], r4
 952 01be FD 2A 25                      MOV.L [r2+], r5
 953 01c1 FD 05 45                      MACLO r4, r5
 954 01c4 FD 04 45                      MACHI r4, r5
 955 01c7 21 F2                         BNE 2b
 956 01c9 2E 1D                         BRA 6f
 957                                    3:
 958 01cb 60 13                         SUB #0x01, r3
 959 01cd 20 12                         BEQ 5f
 960                                    4:
 961 01cf 60 23                         SUB #0x02, r3
 962 01d1 FD 2A 14                      MOV.L [r1+], r4
 963 01d4 FD 2A 25                      MOV.L [r2+], r5
 964 01d7 FD 05 45                      MACLO r4, r5
 965 01da FD 04 45                      MACHI r4, r5
 966 01dd 21 F2                         BNE 4b
 967                                    5:
 968 01df DC 14                         MOV.W [r1], r4
 969 01e1 DC 25                         MOV.W [r2], r5
 970 01e3 FD 05 45                      MACLO r4, r5
 976 01e6 FD 18 10                      RACW #02H
 977 01e9 FD 1F 21                      MVFACMI r1
 983 01ec DF 11                         mov.W   r1, r1      ★ TODO: check
 984 01ee 02                            rts
*/
}

#ifndef set_extb
#define set_extb(data) ccrx_machine_h_set_extb(data)
#endif
__STATIC_FORCEINLINE void ccrx_machine_h_set_extb(void *data)
{
/* CC-RX V2.03 -optimize=2
        MVTC R1, EXTB
        RTS
*/
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVTC %[R1], EXTB"
        : /* OutputOperands */
            /* No outputs. */
        : /* InputOperands */
            [R1] "r" (data)
    );
    return;
/* GNURX 2018q1 -O2
 1014 0204 FD 68 1D                     MVTC r1, EXTB
 1016 0207 02                           rts
 */
}

#ifndef get_extb
#define get_extb() ccrx_machine_h_get_extb()
#endif
__STATIC_FORCEINLINE void *ccrx_machine_h_get_extb(void)
{
/* CC-RX V2.03 -optimize=2
        MVFC EXTB, R1
        RTS
*/
    register void *data;
    __ASM volatile
    (
        /* AssemblerTemplate */
        "MVFC EXTB, %[R1]"
        : /* OutputOperands */
            [R1] "=r" (data)
    );
    return data;
/* GNURX 2018q1 -O2
 1030 0208 FD 6A D1                     MVFC EXTB, r1
 1036 020b 02                           rts
*/
}

#endif /* CCRXMACHINE2_H */

/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of
* this software. By using this software, you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_compiler.h
* Description  : Replace different functions for each compiler.
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : xx.xx.xxxx 1.00     First Release
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#ifndef R_GCOMPILER_H
#define R_GCOMPILER_H

/* ========== Check Compiler ========== */
#if defined(__CCRX__)
    /* supported */
#elif defined(__GNUC__)
    /* supported */
#elif defined(__ICCRX__)
    /* supported */
#else
    #error "Unrecognized compiler"
#endif


/* ========== Macros ========== */
#if defined(__CCRX__)

/* #define __RX 1 */ /* This is already defined by CCRX. */
/* #define __LIT 1 */ /* This is already defined by CCRX. */
/* #define __BIG 1 */ /* This is already defined by CCRX. */

#elif defined(__GNUC__)

#define __RX 1

#if defined(__RX_LITTLE_ENDIAN__)
#define __LIT 1
#else
#define __BIG 1
#endif

#elif defined(__ICCRX__)

#define __RX 1

/* #define __LIT 1 */ /* This is already defined by ICCRX. */
/* #define __BIG 1 */ /* This is already defined by ICCRX. */

#endif


/* ========== Keywords ========== */
#define R_PRAGMA(...)    _Pragma(#__VA_ARGS__)

#if defined(__CCRX__)

/* #define volatile        volatile __evenaccess */

#elif defined(__GNUC__)

#define __evenaccess    /* none */

#elif defined(__ICCRX__)

#define __evenaccess    __sfr

#endif


/* ========== #pragma Directive ========== */

/* ---------- Section Switch ---------- */
#if defined(__CCRX__)

#define R_ATTRIB_SECTION_CHANGE_V(section_name)    R_PRAGMA(section section_name)
#define R_ATTRIB_SECTION_CHANGE_F(section_name)    R_PRAGMA(section section_name)
#define R_ATTRIB_SECTION_CHANGE_END                R_PRAGMA(section)

#elif defined(__GNUC__)

#define R_ATTRIB_SECTION_CHANGE_V(section_name)    __attribute__((section(#section_name)))
#define R_ATTRIB_SECTION_CHANGE_F(section_name)    __attribute__((section(#section_name)))
#define R_ATTRIB_SECTION_CHANGE_END                /* none */

#elif defined(__ICCRX__)

#define R_ATTRIB_SECTION_CHANGE_V(section_name)    R_PRAGMA(location=#section_name)\
                                                   __no_init
#define R_ATTRIB_SECTION_CHANGE_F(section_name)    R_PRAGMA(location=#section_name)
#define R_ATTRIB_SECTION_CHANGE_END                /* none */

#endif

/* ---------- Interrupt Function Creation ---------- */
#if defined(__CCRX__)

#define R_PRAGMA_INTERRUPT(function_name, vector)            R_PRAGMA(interrupt function_name(vect=vector))\
                                                             extern void function_name(void);
#define R_PRAGMA_STATIC_INTERRUPT(function_name, vector)     R_PRAGMA(interrupt function_name(vect=vector))\
                                                             static void function_name(void);

#define R_PRAGMA_INTERRUPT_DEFAULT(function_name)            R_PRAGMA(interrupt function_name)\
                                                             extern void function_name(void);
#define R_PRAGMA_STATIC_INTERRUPT_DEFAULT(function_name)     R_PRAGMA(interrupt function_name)\
                                                             static void function_name(void);

#define R_PRAGMA_INTERRUPT_FUNCTION(function_name)           R_PRAGMA(interrupt function_name)\
                                                             extern void function_name(void);
#define R_PRAGMA_STATIC_INTERRUPT_FUNCTION(function_name)    R_PRAGMA(interrupt function_name)\
                                                             static void function_name(void);

#define R_PRAGMA_FAST_INTERRUPT(function_name)               R_PRAGMA(interrupt function_name(fast))\
                                                             extern void function_name(void);
#define R_PRAGMA_STATIC_FAST_INTERRUPT(function_name)        R_PRAGMA(interrupt function_name(fast))\
                                                             static void function_name(void);

#define R_ATTRIB_INTERRUPT                                   extern
#define R_ATTRIB_STATIC_INTERRUPT                            static

#define R_ATTRIB_FAST_INTERRUPT                              extern
#define R_ATTRIB_STATIC_FAST_INTERRUPT                       static

#elif defined(__GNUC__)

#define R_PRAGMA_INTERRUPT(function_name, vector)            extern void function_name(void) __attribute__((interrupt(".rvectors", vector)));
#define R_PRAGMA_STATIC_INTERRUPT(function_name, vector)     static void function_name(void) __attribute__((interrupt(".rvectors", vector), used));

#define R_PRAGMA_INTERRUPT_DEFAULT(function_name)            extern void function_name(void) __attribute__((interrupt(".rvectors", "$default")));
#define R_PRAGMA_STATIC_INTERRUPT_DEFAULT(function_name)     static void function_name(void) __attribute__((interrupt(".rvectors", "$default"), used));

#define R_PRAGMA_INTERRUPT_FUNCTION(function_name)           extern void function_name(void) __attribute__((interrupt));
#define R_PRAGMA_STATIC_INTERRUPT_FUNCTION(function_name)    static void function_name(void) __attribute__((interrupt, used));

#define R_PRAGMA_FAST_INTERRUPT(function_name)               extern void function_name(void) __attribute__((interrupt(fast)));
#define R_PRAGMA_STATIC_FAST_INTERRUPT(function_name)        static void function_name(void) __attribute__((interrupt(fast)), used);

#define R_ATTRIB_INTERRUPT                                   extern
#define R_ATTRIB_STATIC_INTERRUPT                            static

#define R_ATTRIB_FAST_INTERRUPT                              extern
#define R_ATTRIB_STATIC_FAST_INTERRUPT                       static

#elif defined(__ICCRX__)

#define R_PRAGMA_INTERRUPT(function_name, vect)              R_PRAGMA(vector=vect)\
                                                             extern __interrupt void function_name(void);
#define R_PRAGMA_STATIC_INTERRUPT(function_name, vect)       R_PRAGMA(vector=vect)\
                                                             static __interrupt void function_name(void);

#define R_PRAGMA_INTERRUPT_DEFAULT(function_name)            extern __interrupt void function_name(void);
#define R_PRAGMA_STATIC_INTERRUPT_DEFAULT(function_name)     static __interrupt void function_name(void);

#define R_PRAGMA_INTERRUPT_FUNCTION(function_name)           extern __interrupt void function_name(void);
#define R_PRAGMA_STATIC_INTERRUPT_FUNCTION(function_name)    static __interrupt void function_name(void);

#define R_PRAGMA_FAST_INTERRUPT(function_name)               extern __fast_interrupt void function_name(void);
#define R_PRAGMA_STATIC_FAST_INTERRUPT(function_name)        static __fast_interrupt void function_name(void);

#define R_ATTRIB_INTERRUPT                                   extern __interrupt
#define R_ATTRIB_STATIC_INTERRUPT                            static __interrupt

#define R_ATTRIB_FAST_INTERRUPT                              extern __fast_interrupt
#define R_ATTRIB_STATIC_FAST_INTERRUPT                       static __fast_interrupt

#endif

/* ---------- Inline Expansion of Function ---------- */
#if defined(__CCRX__)

#define R_PRAGMA_INLINE(function_name)           R_PRAGMA(inline function_name)\
                                                 extern

#define R_PRAGMA_STATIC_INLINE(function_name)    R_PRAGMA(inline function_name)\
                                                 static

#elif defined(__GNUC__)

#define R_PRAGMA_INLINE(function_name)           inline extern __attribute__((always_inline))

#define R_PRAGMA_STATIC_INLINE(function_name)    inline static __attribute__((always_inline))

#elif defined(__ICCRX__)

#define R_PRAGMA_INLINE(function_name)           inline extern

#define R_PRAGMA_STATIC_INLINE(function_name)    inline static

#endif

/* ---------- Inline Expansion of Assembly-Language Function ---------- */
#if defined(__CCRX__)

#define R_PRAGMA_INLINE_ASM(function_name)           R_PRAGMA(inline_asm function_name)\
                                                     extern

#define R_PRAGMA_INLINE_STATIC_ASM(function_name)    R_PRAGMA(inline_asm function_name)\
                                                     static

#elif defined(__GNUC__)

#define R_PRAGMA_INLINE_ASM(function_name)           extern
#define R_PRAGMA_INLINE_STATIC_ASM(function_name)    static

#elif defined(__ICCRX__)

#define R_PRAGMA_INLINE_ASM(function_name)           extern
#define R_PRAGMA_INLINE_STATIC_ASM(function_name)    static

#endif

/* ---------- Bit Field Order Specification ---------- */
#if defined(__CCRX__)

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_2(bf1, bf2)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_3(bf1, bf2, bf3)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_4(bf1, bf2, bf3, bf4)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_5(bf1, bf2, bf3, bf4, bf5)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_6(bf1, bf2, bf3, bf4, bf5, bf6)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_7(bf1, bf2, bf3, bf4, bf5, bf6, bf7)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_8(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_9(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_10(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_11(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_12(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_13(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_14(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_15(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_16(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_17(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_18(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_19(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_20(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_21(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_22(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_23(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_24(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_25(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_26(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_27(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_28(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_29(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_30(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_31(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30, \
                                            bf31)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
        bf31;\
    };\
R_PRAGMA(bit_order)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_32(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30, \
                                            bf31, bf32)\
struct {\
R_PRAGMA(bit_order left)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
        bf31;\
        bf32;\
    };\
R_PRAGMA(bit_order)\
}

#elif defined(__GNUC__)

#if defined(__RX_LITTLE_ENDIAN__)

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_2(bf1, bf2)\
struct {\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_3(bf1, bf2, bf3)\
struct {\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_4(bf1, bf2, bf3, bf4)\
struct {\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_5(bf1, bf2, bf3, bf4, bf5)\
struct {\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_6(bf1, bf2, bf3, bf4, bf5, bf6)\
struct {\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_7(bf1, bf2, bf3, bf4, bf5, bf6, bf7)\
struct {\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_8(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8)\
struct {\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_9(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9)\
struct {\
    bf9;\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_10(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10)\
struct {\
    bf10;\
    bf9;\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_11(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11)\
struct {\
    bf11;\
    bf10;\
    bf9;\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_12(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12)\
struct {\
    bf12;\
    bf11;\
    bf10;\
    bf9;\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_13(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13)\
struct {\
    bf13;\
    bf12;\
    bf11;\
    bf10;\
    bf9;\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_14(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14)\
struct {\
    bf14;\
    bf13;\
    bf12;\
    bf11;\
    bf10;\
    bf9;\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_15(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15)\
struct {\
    bf15;\
    bf14;\
    bf13;\
    bf12;\
    bf11;\
    bf10;\
    bf9;\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_16(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16)\
struct {\
    bf16;\
    bf15;\
    bf14;\
    bf13;\
    bf12;\
    bf11;\
    bf10;\
    bf9;\
    bf8;\
    bf7;\
    bf6;\
    bf5;\
    bf4;\
    bf3;\
    bf2;\
    bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_17(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17)\
struct {\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_18(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18)\
struct {\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_19(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19)\
struct {\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_20(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20)\
struct {\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_21(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21)\
struct {\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_22(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22)\
struct {\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_23(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23)\
struct {\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_24(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24)\
struct {\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_25(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25)\
struct {\
        bf25;\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_26(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26)\
struct {\
        bf26;\
        bf25;\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_27(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27)\
struct {\
        bf27;\
        bf26;\
        bf25;\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_28(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28)\
struct {\
        bf28;\
        bf27;\
        bf26;\
        bf25;\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_29(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29)\
struct {\
        bf29;\
        bf28;\
        bf27;\
        bf26;\
        bf25;\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_30(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30)\
struct {\
        bf30;\
        bf29;\
        bf28;\
        bf27;\
        bf26;\
        bf25;\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_31(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30, \
                                            bf31)\
struct {\
        bf31;\
        bf30;\
        bf29;\
        bf28;\
        bf27;\
        bf26;\
        bf25;\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_32(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30, \
                                            bf31, bf32)\
struct {\
        bf32;\
        bf31;\
        bf30;\
        bf29;\
        bf28;\
        bf27;\
        bf26;\
        bf25;\
        bf24;\
        bf23;\
        bf22;\
        bf21;\
        bf20;\
        bf19;\
        bf18;\
        bf17;\
        bf16;\
        bf15;\
        bf14;\
        bf13;\
        bf12;\
        bf11;\
        bf10;\
        bf9;\
        bf8;\
        bf7;\
        bf6;\
        bf5;\
        bf4;\
        bf3;\
        bf2;\
        bf1;\
}

#else

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_2(bf1, bf2)\
struct {\
    bf1;\
    bf2;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_3(bf1, bf2, bf3)\
struct {\
    bf1;\
    bf2;\
    bf3;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_4(bf1, bf2, bf3, bf4)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_5(bf1, bf2, bf3, bf4, bf5)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_6(bf1, bf2, bf3, bf4, bf5, bf6)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_7(bf1, bf2, bf3, bf4, bf5, bf6, bf7)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_8(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_9(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
    bf9;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_10(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
    bf9;\
    bf10;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_11(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
    bf9;\
    bf10;\
    bf11;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_12(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
    bf9;\
    bf10;\
    bf11;\
    bf12;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_13(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
    bf9;\
    bf10;\
    bf11;\
    bf12;\
    bf13;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_14(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
    bf9;\
    bf10;\
    bf11;\
    bf12;\
    bf13;\
    bf14;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_15(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
    bf9;\
    bf10;\
    bf11;\
    bf12;\
    bf13;\
    bf14;\
    bf15;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_16(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16)\
struct {\
    bf1;\
    bf2;\
    bf3;\
    bf4;\
    bf5;\
    bf6;\
    bf7;\
    bf8;\
    bf9;\
    bf10;\
    bf11;\
    bf12;\
    bf13;\
    bf14;\
    bf15;\
    bf16;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_17(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_18(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_19(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_20(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_21(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_22(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_23(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_24(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_25(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_26(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_27(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_28(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_29(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_30(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_31(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30, \
                                            bf31)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
        bf31;\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_32(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                            bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                            bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30, \
                                            bf31, bf32)\
struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
        bf31;\
        bf32;\
}

#endif

#elif defined(__ICCRX__)

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_2(bf1, bf2)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_3(bf1, bf2, bf3)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_4(bf1, bf2, bf3, bf4)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_5(bf1, bf2, bf3, bf4, bf5)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_6(bf1, bf2, bf3, bf4, bf5, bf6)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_7(bf1, bf2, bf3, bf4, bf5, bf6, bf7)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_8(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_9(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_10(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_11(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_12(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_13(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_14(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_15(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_16(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_17(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_18(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_19(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_20(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_21(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_22(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_23(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_24(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_25(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24, bf25)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_26(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24, bf25, bf26)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_27(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24, bf25, bf26, bf27)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_28(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_29(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_30(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_31(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30, \
                                           bf31)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
        bf31;\
    };\
R_PRAGMA(bitfields=default)\
}

#define R_ATTRIB_BITFIELD_BIT_ORDER_LEFT_32(bf1, bf2, bf3, bf4, bf5, bf6, bf7, bf8, bf9, bf10, \
                                           bf11, bf12, bf13, bf14, bf15, bf16, bf17, bf18, bf19, bf20, \
                                           bf21, bf22, bf23, bf24, bf25, bf26, bf27, bf28, bf29, bf30, \
                                           bf31, bf32)\
struct {\
R_PRAGMA(bitfields=reversed_disjoint_types)\
    struct {\
        bf1;\
        bf2;\
        bf3;\
        bf4;\
        bf5;\
        bf6;\
        bf7;\
        bf8;\
        bf9;\
        bf10;\
        bf11;\
        bf12;\
        bf13;\
        bf14;\
        bf15;\
        bf16;\
        bf17;\
        bf18;\
        bf19;\
        bf20;\
        bf21;\
        bf22;\
        bf23;\
        bf24;\
        bf25;\
        bf26;\
        bf27;\
        bf28;\
        bf29;\
        bf30;\
        bf31;\
        bf32;\
    };\
R_PRAGMA(bitfields=default)\
}

#endif

/* ---------- Alignment Value Specification for Structure Members and Class Members ---------- */
#if defined(__CCRX__)

#define R_PRAGMA_PACK          R_PRAGMA(pack)
#define R_PRAGMA_UNPACK        R_PRAGMA(unpack)
#define R_PRAGMA_PACKOPTION    R_PRAGMA(packoption)

#elif defined(__GNUC__)

#define R_PRAGMA_PACK          R_PRAGMA(pack(1))
#define R_PRAGMA_UNPACK        R_PRAGMA(pack(4))
#define R_PRAGMA_PACKOPTION    R_PRAGMA(pack())

#elif defined(__ICCRX__)

#define R_PRAGMA_PACK          R_PRAGMA(pack(1))
#define R_PRAGMA_UNPACK        R_PRAGMA(pack(4))
#define R_PRAGMA_PACKOPTION    R_PRAGMA(pack())

#endif


/* ========== Intrinsic Functions ========== */
#if defined(__CCRX__)

/* ---------- Data Exchange ---------- */
#define R_EXCHANGE_CAST_ARGS1    signed long *
#define R_EXCHANGE_CAST_ARGS2    signed long *
#define R_EXCHANGE(x, y)         xchg(x, y)    /* void xchg(signed long *data1, signed long *data2) */

/* ---------- Special Instructions ---------- */
#define R_BRK()    brk()    /* void brk(void) */
#define R_WAIT()   wait()    /* void wait(void) */
#define R_NOP()    nop()    /* void nop(void) */

/* ---------- Processor interrupt priority level (IPL) ---------- */
//#define R_SET_IPL_CAST_ARGS1    signed long
//#define R_SET_IPL(x)            set_ipl(x)    /* void set_ipl(signed long level) */

//#define R_GET_IPL_CAST_RET      signed long
//#define R_GET_IPL()             get_ipl()    /* unsigned char get_ipl(void) */

/* ---------- Processor status word (PSW) ---------- */
#define R_SET_PSW_CAST_ARGS1    unsigned long
#define R_SET_PSW(x)            set_psw(x)    /* void set_psw(unsigned long data) */

#define R_GET_PSW_CAST_RET      unsigned long
#define R_GET_PSW()             get_psw()    /* unsigned long get_psw(void) */

/* ---------- Floating-point status word (FPSW) ---------- */
#define R_SET_FPSW_CAST_ARGS1    unsigned long
#define R_SET_FPSW(x)            set_fpsw(x)    /* void set_fpsw(unsigned long data) */

#define R_GET_FPSW_CAST_RET      unsigned long
#define R_GET_FPSW()             get_fpsw()    /* unsigned long get_fpsw(void) */

/* ---------- Processor mode (PM) ---------- */
#define R_CHG_PMUSR()    chg_pmusr()    /* void chg_pmusr(void) */

/* ---------- Control of the interrupt enable bits ---------- */
#define R_SETPSW_I()    setpsw_i()    /* void setpsw_i(void) */
#define R_CLRPSW_I()    clrpsw_i()    /* void clrpsw_i(void) */

#elif defined(__GNUC__)

/* ---------- Data Exchange ---------- */
#define R_EXCHANGE_CAST_ARGS1    int *
#define R_EXCHANGE_CAST_ARGS2    int *
#define R_EXCHANGE(x, y)         __builtin_rx_xchg(x, y)    /* void __builtin_rx_xchg (int *, int *) */

/* ---------- Special Instructions ---------- */
#define R_BRK()    __asm("brk")
#define R_WAIT()   __asm("wait")
#define R_NOP()    __asm("nop")

/* ---------- Processor interrupt priority level (IPL) ---------- */
//#define R_SET_IPL_CAST_ARGS1    /* none */
//#define R_SET_IPL(x)            /* none */

//#define R_GET_IPL_CAST_RET      /* none */
//#define R_GET_IPL()             /* none */

/* ---------- Processor status word (PSW) ---------- */
#define R_SET_PSW_CAST_ARGS1    int
#define R_SET_PSW(x)            __builtin_rx_mvtc(0x0, x)    /* void __builtin_rx_mvtc (int reg, int val) */

#define R_GET_PSW_CAST_RET      int
#define R_GET_PSW()             __builtin_rx_mvfc(0x0)    /* int __builtin_rx_mvfc (int) */

/* ---------- Floating-point status word (FPSW) ---------- */
#define R_SET_FPSW_CAST_ARGS1    int
#define R_SET_FPSW(x)            __builtin_rx_mvtc(0x3, x)    /* void __builtin_rx_mvtc (int reg, int val) */

#define R_GET_FPSW_CAST_RET      int
#define R_GET_FPSW()             __builtin_rx_mvfc(0x3)    /* int __builtin_rx_mvfc (int) */

/* ---------- Processor mode (PM) ---------- */
#define R_CHG_PMUSR()    Change_PSW_PM_to_UserMode()

/* ---------- Control of the interrupt enable bits ---------- */
#define R_SETPSW_I()    __builtin_rx_setpsw('I')    /* void __builtin_rx_setpsw (int) */
#define R_CLRPSW_I()    __builtin_rx_clrpsw('I')    /* void __builtin_rx_setpsw (int) */

#elif defined(__ICCRX__)

/* ---------- Data Exchange ---------- */
#define R_EXCHANGE_CAST_ARGS1    signed long *
#define R_EXCHANGE_CAST_ARGS2    signed long *
#define R_EXCHANGE(x, y)         xchg(x, y)    /* void xchg(signed long *data1, signed long *data2) */

/* ---------- Special Instructions ---------- */
#define R_BRK()    __break()    /* void __break(void) */
#define R_WAIT()   __wait_for_interrupt()    /* void __wait_for_interrupt(void) */
#define R_NOP()    __no_operation()    /* void __no_operation(void) */

/* ---------- Processor interrupt priority level (IPL) ---------- */
//#define R_SET_IPL_CAST_ARGS1    __ilevel_t
//#define R_SET_IPL(x)            __set_interrupt_level(x)    /* void __set_interrupt_level(__ilevel_t) */

//#define R_GET_IPL_CAST_RET      __ilevel_t
//#define R_GET_IPL()             __get_interrupt_level()    /* __ilevel_t __get_interrupt_level(void) */

/* ---------- Processor status word (PSW) ---------- */
#define R_SET_PSW_CAST_ARGS1    unsigned long
#define R_SET_PSW(x)            set_psw(x)    /* void set_psw(unsigned long data) */

#define R_GET_PSW_CAST_RET      unsigned long
#define R_GET_PSW()             get_psw()    /* unsigned long get_psw(void) */

/* ---------- Floating-point status word (FPSW) ---------- */
#define R_SET_FPSW_CAST_ARGS1    unsigned long
#define R_SET_FPSW(x)            set_fpsw(x)    /* void set_fpsw(unsigned long data) */

#define R_GET_FPSW_CAST_RET      unsigned long
#define R_GET_FPSW()             get_fpsw()    /* unsigned long get_fpsw(void) */

/* ---------- Processor mode (PM) ---------- */
#define R_CHG_PMUSR()    Change_PSW_PM_to_UserMode()

/* ---------- Control of the interrupt enable bits ---------- */
#define R_SETPSW_I()    __enable_interrupt()    /* void __enable_interrupt(void) */
#define R_CLRPSW_I()    __disable_interrupt()    /* void __disable_interrupt(void) */

#endif

#endif /* R_GCOMPILER_H */

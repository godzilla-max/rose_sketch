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
 * Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_mcu_config_reference.h
 * Description  : Defines the FLASH MCU configuration when not using the FIT BSP.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           11.07.2016 1.00    First Release
*           24.01.2017 1.10    Added equates for RSKRX65N-2
*           02.08.2017 1.10    Moved FLASH_CFG_USE_FIT_BSP == 1 case to r_flash_targets.h.
*           xx.xx.xxxx x.xx    Added support for GNUC and ICCRX.
***********************************************************************************************************************/
#ifndef R_MCU_CONFIG_HEADER_FILE
#define R_MCU_CONFIG_HEADER_FILE

#include "r_flash_rx_config.h"

#if (FLASH_CFG_USE_FIT_BSP == 0)

#include <stdint.h>     // typedefs
#include <stdbool.h>
#include <stddef.h>     // NULL
#if defined(__CCRX__) || defined(__ICCRX__)
/* Intrinsic functions provided by compiler. */
#include <machine.h>
#elif defined(__GNUC__)
/* No header file for intrinsic functions. */
#endif
#include "iodefine.h"

#define MCU_RX65N
#define MCU_RX65_ALL

/****************************************************************************************
 MCU Configuration when not using r_bsp:
 Configuration options are set here
 Change the values of the following defines as needed.
 DON'T FORGET TO SELECT CLOCK (if LOCO not desired) AND
 ENABLE GLOBAL INTERRUPTS IN YOUR MAIN APPLICATION (normally done by BSP).
****************************************************************************************/

#define MCU_CFG_ICLK_HZ             (120000000)
#define MCU_CFG_FCLK_HZ             (60000000)

/* ROM, RAM, and Data Flash Capacity.
   Character(s) = Value for macro = ROM Size/Ram Size/Data Flash Size
   4            = 0x4             = 512KB/256KB/Not equipped
   7            = 0x7             = 768KB/256KB/Not equipped
   9            = 0x9             = 1MB/256KB/Not equipped  - RSK+RX65N-1
   C            = 0xC             = 1.5MB/640KB/32KB
   E            = 0xE             = 2MB/640KB/32KB          - RSK+RX65N-2
   NOTE: When the RAM capacity is 640KB, the RAM areas are not contiguous
*/
#define MCU_CFG_PART_MEMORY_SIZE    (0x9)

/****************************************************************************************
 Do not change the following values.
****************************************************************************************/
#if (MCU_CFG_PART_MEMORY_SIZE == 0x4)
    #define MCU_ROM_SIZE_BYTES              (524288)
    #define MCU_RAM_SIZE_BYTES              (262144)
    #define MCU_DATA_FLASH_SIZE_BYTES       (0)
#elif (MCU_CFG_PART_MEMORY_SIZE == 0x7)
    #define MCU_ROM_SIZE_BYTES              (786432)
    #define MCU_RAM_SIZE_BYTES              (262144)
    #define MCU_DATA_FLASH_SIZE_BYTES       (0)
#elif (MCU_CFG_PART_MEMORY_SIZE == 0x9)
    #define MCU_ROM_SIZE_BYTES              (1048576)
    #define MCU_RAM_SIZE_BYTES              (262144)
    #define MCU_DATA_FLASH_SIZE_BYTES       (0)
#elif (MCU_CFG_PART_MEMORY_SIZE == 0xC)
    #define MCU_ROM_SIZE_BYTES              (1572864)
    #define MCU_RAM_SIZE_BYTES              (655360)
    #define MCU_DATA_FLASH_SIZE_BYTES       (32768)
#elif (MCU_CFG_PART_MEMORY_SIZE == 0xE)
    #define MCU_ROM_SIZE_BYTES              (2097152)
    #define MCU_RAM_SIZE_BYTES              (655360)
    #define MCU_DATA_FLASH_SIZE_BYTES       (32768)
#else
    #error "ERROR - MCU_CFG_PART_MEMORY_SIZE - Unknown memory size chosen in r_mcu_config.h"
#endif

#define FIT_NO_PTR      ((void *)0x10000000)    // Reserved space on RX
#define FIT_NO_FUNC     ((void *)0x10000000)    // Reserved space on RX

/* RSKRX65N-1 equates for reference. Change to #if 1 if want to use. */
#if 0
#define LED_ON          (0)
#define LED_OFF         (1)
#define LED0            PORT0.PODR.BIT.B3
#define LED1            PORT0.PODR.BIT.B5
#define LED2            PORT7.PODR.BIT.B3
#define LED3            PORTJ.PODR.BIT.B5
#define SW1             PORT0.PIDR.BIT.B0
#define SW2             PORT0.PIDR.BIT.B1
#define SW3             PORT0.PIDR.BIT.B7
#endif

/* RSKRX65N-2 equates for reference. Change to #if 1 if want to use. */
#if 0
#define LED_ON          (0)
#define LED_OFF         (1)
#define LED0            PORT7.PODR.BIT.B3
#define LED1            PORTG.PODR.BIT.B7
#define LED2            PORTG.PODR.BIT.B6
#define LED3            PORTG.PODR.BIT.B5
#define SW1             PORT0.PIDR.BIT.B3
#define SW2             PORT0.PIDR.BIT.B5
#define SW3             PORT0.PIDR.BIT.B7
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

#define R_ATTRIB_INTERRUPT                                   extern /* only this one because of no corresponding keyword */
#define R_ATTRIB_STATIC_INTERRUPT                            static /* only this one because of no corresponding keyword */

#define R_ATTRIB_FAST_INTERRUPT                              extern /* only this one because of no corresponding keyword */
#define R_ATTRIB_STATIC_FAST_INTERRUPT                       static /* only this one because of no corresponding keyword */

#elif defined(__GNUC__)

#define R_PRAGMA_INTERRUPT(function_name, vector)            extern void function_name(void) __attribute__((interrupt(".rvectors", vector)));
#define R_PRAGMA_STATIC_INTERRUPT(function_name, vector)     static void function_name(void) __attribute__((interrupt(".rvectors", vector), used));

#define R_PRAGMA_INTERRUPT_DEFAULT(function_name)            extern void function_name(void) __attribute__((interrupt(".rvectors", "$default")));
#define R_PRAGMA_STATIC_INTERRUPT_DEFAULT(function_name)     static void function_name(void) __attribute__((interrupt(".rvectors", "$default"), used));

#define R_PRAGMA_INTERRUPT_FUNCTION(function_name)           extern void function_name(void) __attribute__((interrupt));
#define R_PRAGMA_STATIC_INTERRUPT_FUNCTION(function_name)    static void function_name(void) __attribute__((interrupt, used));

#define R_PRAGMA_FAST_INTERRUPT(function_name)               extern void function_name(void) __attribute__((interrupt(fast)));
#define R_PRAGMA_STATIC_FAST_INTERRUPT(function_name)        static void function_name(void) __attribute__((interrupt(fast)), used);

#define R_ATTRIB_INTERRUPT                                   extern /* only this one because __attribute__((interrupt)) prevents GNURX from generating vector */
#define R_ATTRIB_STATIC_INTERRUPT                            static /* only this one because __attribute__((interrupt, used)) prevents GNURX from generating vector */

#define R_ATTRIB_FAST_INTERRUPT                              extern /* __attribute__((interrupt(fast))) Not necessary, but Don't forget a R_PRAGMA_FAST_INTERRUPT() declaration */
#define R_ATTRIB_STATIC_FAST_INTERRUPT                       static /* __attribute__((interrupt(fast)), used) Not necessary, but Don't forget a R_PRAGMA_STATIC_FAST_INTERRUPT() declaration */

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

#define R_ATTRIB_INTERRUPT                                   extern __interrupt /* ICCRX requires __interrupt not only at a function declaration but also at a function definition */
#define R_ATTRIB_STATIC_INTERRUPT                            static __interrupt /* ICCRX requires __interrupt not only at a function declaration but also at a function definition */

#define R_ATTRIB_FAST_INTERRUPT                              extern __fast_interrupt /* ICCRX requires __interrupt not only at a function declaration but also at a function definition */
#define R_ATTRIB_STATIC_FAST_INTERRUPT                       static __fast_interrupt /* ICCRX requires __interrupt not only at a function declaration but also at a function definition */

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
#define R_NOP()    nop()    /* void nop(void) */

#elif defined(__GNUC__)

/* ---------- Data Exchange ---------- */
#define R_EXCHANGE_CAST_ARGS1    int *
#define R_EXCHANGE_CAST_ARGS2    int *
#define R_EXCHANGE(x, y)         __builtin_rx_xchg(x, y)    /* void __builtin_rx_xchg (int *, int *) */

/* ---------- Special Instructions ---------- */
#define R_NOP()    __asm("nop")

#elif defined(__ICCRX__)

/* ---------- Data Exchange ---------- */
#define R_EXCHANGE_CAST_ARGS1    signed long *
#define R_EXCHANGE_CAST_ARGS2    signed long *
#define R_EXCHANGE(x, y)         xchg(x, y)    /* void xchg(signed long *data1, signed long *data2) */

/* ---------- Special Instructions ---------- */
#define R_NOP()    __no_operation()    /* void __no_operation(void) */

#endif

#endif /* R_BSP Module not used */
#endif /* R_MCU_CONFIG_HEADER_FILE */

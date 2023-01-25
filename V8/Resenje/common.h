#ifndef _COMMON_H
#define _COMMON_H

#include <stdfix.h>

// potrebno prekopirati sa pocetka stdfix_emu.h ili ukljuciti ceo stdfix_emu.h!
#if defined(__CCC)

#include <stdfix.h>

#define FRACT_NUM(x) (x##r)
#define LONG_FRACT_NUM(x) (x##lr)
#define ACCUM_NUM(x) (x##lk)

#define FRACT_NUM_HEX(x) (x##r)

#define FRACT_TO_INT_BIT_CONV(x) (bitsr(x))
#define INT_TO_FRACT_BIT_CONV(x) (rbits(x))

#define long_accum long accum
#define long_fract long fract

#endif

#define GAINPROC_ASM
/////////////////////////////////////////////////////////////////////////////////
// Constant definitions
#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8

#define INPUT_NUM_CHANNELS 2
#define OUTPUT_NUM_CHANNELS 3

#define LEFT_CH 0
#define RIGHT_CH 1
#define CENTER_CH 2

#define PLUS_6DB FRACT_NUM(0.99763) //1.99526 ORIGINAL VALUE
#define MINUS_2DB FRACT_NUM(0.794328)
#define MINUS_4DB FRACT_NUM(0.630957)
#define MINUS_12DB FRACT_NUM(0.251189)
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////

/* DSP type definitions */
typedef short DSPshort;					/* DSP integer */
typedef unsigned short DSPushort;		/* DSP unsigned integer */
typedef int DSPint;						/* native integer */
typedef fract DSPfract;					/* DSP fixed-point fractional, data reg, memory word format s.31 */
typedef long_accum DSPaccum;					/* DSP fixed-point fractional, accum reg format s8.63 */

#endif //_COMMON_H

#ifndef COMMON_H
#define COMMON_H

#include <stdfix.h>

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

/* Basic constants */
#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8

// Number of channels
#define INPUT_NUM_CHANNELS 2
#define DEFAULT_OUTPUT_NUM_CHANNELS 5
#define OUTPUT_NUM_CHANNELS_2 2

#define PROCESSING_ASM
// Channel IDs. 
// Should input and output channel IDs be separated?
#define LEFT_CH 0
#define RIGHT_CH 1
#define LS_CH 2
#define RS_CH 3
#define C_CH 4

// Gain linear values.
#define MINUS_4DB FRACT_NUM(0x50c335d4)
#define PLUS_4DB FRACT_NUM(0x0caddc7b) 		// 1.58489/16
#define MINUS_0DB FRACT_NUM(1.0)
#define PLUS_0DB FRACT_NUM(0.0625)			// 1.0/16
#define MINUS_24DB FRACT_NUM(0.0630957)
#define PLUS_24DB FRACT_NUM(0.99055625) 	// 15.8489/16

// Saturation threshold
#define THRESHOLD FRACT_NUM(0.999)

// Filter coeffs
#define FILTERLEN 31

typedef fract DSPfract; 
typedef long_accum DSPaccum; 
typedef int DSPint; 


#endif

#ifndef COMMON_H
#define COMMON_H

/* Basic constants */
/* TO DO: Move defined constants here */
/////////////////////////////////////////////////////////////////////////////////
// Constant definitions
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////

/* DSP type definitions */
/* DSP integer */
/* DSP unsigned integer */
/* native integer */
/* DSP fixed-point fractional */
/* DSP fixed-point fractional */

#include "stdfix_emu.h"

#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8

// Number of channels
#define INPUT_NUM_CHANNELS 2
#define OUTPUT_NUM_CHANNELS 3

// Channel IDs. 
// Should input and output channel IDs be separated?
#define LEFT_CH 0
#define RIGHT_CH 1
#define CENTER_CH 2

// Gain linear values. 
#define PLUS_6DB FRACT_NUM(0.99763) // original value 1.99526
#define MINUS_2DB FRACT_NUM(0.794328)
#define MINUS_4DB FRACT_NUM(0.630957)
#define MINUS_12DB FRACT_NUM(0.251189)

typedef fract DSPfract;
typedef long_accum DSPaccum;
typedef int DSPint;

#endif

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
#define DEFAULT_OUTPUT_NUM_CHANNELS 5
#define OUTPUT_NUM_CHANNELS_2 2

// Channel IDs. 
// Should input and output channel IDs be separated?
#define LEFT_CH 0
#define RIGHT_CH 1
#define LS_CH 2
#define RS_CH 3
#define C_CH 4

// Gain linear values. 
#define MINUS_4DB 0.630957
#define PLUS_4DB 1.58489/16

// Saturation threshold
#define THRESHOLD 0.999

// Filter coeffs
#define FILTERLEN 31

typedef fract DSPfract;
typedef long_accum DSPaccum;
typedef int DSPint;

#endif

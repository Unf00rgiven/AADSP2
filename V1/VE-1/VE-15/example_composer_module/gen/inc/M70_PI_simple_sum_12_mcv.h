/*
 * This file is automatically generated by CLIDE
 *
 * MCV (Module Control Vector)
 */

#include <dsplib/platform_support.h>
#include <dsplib/cl_os.h> 
#include <dsplib/meter.h>
#include <dsplib/response.h>
#include <example_composer_module_common.h>

#define	M70_PI_simple_sum_12_default_right_shift_count 	 0


typedef struct {
	int shift_value;	 //int
} M70_PI_simple_sum_12_PUBLIC_DATA_t;

typedef struct {
} M70_PI_simple_sum_12_PRIVATE_DATA_2_t;

typedef struct {
	int __memX* bypass;	 //int __memX*
	fract __memY* __memY* audio_brick_out;	 //fract(1.31) __memY* __memY*
	fract __memY* __memY* audio_brick_in[2];	 //fract(1.31) __memY* __memY*
} M70_PI_simple_sum_12_PRIVATE_DATA_1_t;





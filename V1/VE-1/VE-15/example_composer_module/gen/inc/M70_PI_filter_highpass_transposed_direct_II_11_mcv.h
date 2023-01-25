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

#define	M70_PI_filter_highpass_transposed_direct_II_11_max_order 	 4
#define	M70_PI_filter_highpass_transposed_direct_II_11_precision 	 0

//--------------- extern_decls.h -----------------
/*
 * add any externs that might be necessary when a module using this primitive
 * is compiled.
 */
#ifndef __FILTER_PASS_TRANSPOSED_DIRECT_II_BIQUAD_EXTERN_DECLS_H_
#define __FILTER_PASS_TRANSPOSED_DIRECT_II_BIQUAD_EXTERN_DECLS_H_
extern void cl_biquad_prim_register_coeffs();
extern void cl_biquad_prim_register_coeffs_dp();

#endif

//------------------------------------------------


typedef struct {
	fract a;	 //fract(1.31)
	fract b;	 //fract(1.31)
	int c;	 //int
} M70_PI_filter_highpass_transposed_direct_II_11_biquad_coeffs_t_t;

typedef struct {
	long long a;	 //long long
	long long b;	 //long long
	long long w1;	 //long long
	long long w2;	 //long long
} M70_PI_filter_highpass_transposed_direct_II_11_biquad_state_t_t;

typedef struct {
	int hold_count;	 //int
	fract ramp;	 //fract(1.31)
	fract k;	 // fract(2.30)	gain factor
	M70_PI_filter_highpass_transposed_direct_II_11_biquad_coeffs_t_t structure_instance[2];	 //biquad_coeffs_t
	bool clip;	 //bool
} M70_PI_filter_highpass_transposed_direct_II_11_PUBLIC_DATA_t;

typedef struct {
	long long k;	 //long long
	M70_PI_filter_highpass_transposed_direct_II_11_biquad_state_t_t structure_instance[2];	 //biquad_state_t
} M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_2_t;

typedef struct {
	void __memX* pointer;	 //void __memX*
	int __memX* bypass;	 //int __memX*
	fract __memY* __memY* audio_brick_in;	 //fract(1.31) __memY* __memY*
	fract __memY* __memY* audio_brick_out;	 //fract(1.31) __memY* __memY*
	int biquad_loop_count;	 //int
	int myccr;	 //int
	int clear_time;	 //int
	bool __memY* clip_ptr;	 //bool __memY*
} M70_PI_filter_highpass_transposed_direct_II_11_PRIVATE_DATA_1_t;





/*
 * This file is automatically generated by CLIDE
 *
 * example_composer_module common definition file
 */

#ifndef _MODULE_COMMON_
#define _MODULE_COMMON_

#include <dsplib/cl_subkernel_types.h>

/*
 * Subkernel primitive functions
 */
void __fg_call module_preKickstart(__memY void * mif_ptr);
void __fg_call module_postKickstart(void);
void __fg_call module_timer(void);
void __fg_call module_frame(void);
void __fg_call module_block(void);
void __bg_call module_background(void);
void __fg_call module_preMallocInit(void);
void __fg_call module_postMallocInit(void);

#endif
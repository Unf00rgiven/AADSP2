#ifndef PROCESSING_H
#define PROCESSING_H

#include "common.h"

#if __cplusplus
extern "C" {
#endif

enum OUTPUT_MODES
{
	OM_2_0_0 = 0,
	OM_3_0_0 = 1
};

int get_out_num_ch(int mode);

int is_channel_active(int mode, int channel);

void processing(double input[][BLOCK_SIZE], double output[][BLOCK_SIZE], MultitapEcho** mEchos, int mode);

#if __cplusplus
}
#endif

#endif // !PROCESSING_H

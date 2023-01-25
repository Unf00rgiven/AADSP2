
#include "multitap_echo.h"
#include "processing.h"


#define MAX_NUM_OUT_CH		3
#define MAX_NUM_IN_CH		2
#define MAX_NUM_OUT_MODE	2 

// Index of channels
#define L		0
#define R		1
#define C       2

static const int outputChMask[MAX_NUM_OUT_MODE][MAX_NUM_OUT_CH] =
{
	//L  R  C
	{ 1, 1, 0 },	//2.0.0 (stereo) 
	{ 1, 1, 1 }		//3.0.0 (stereo + center)
};

int get_out_num_ch(int mode)
{
	int ret = 0;

	for (int i = 0; i < MAX_NUM_OUT_CH; i++)
	{
		ret += outputChMask[mode][i];
	}

	return ret;
}

int is_channel_active(int mode, int channel)
{
	return outputChMask[mode][channel];
}


/////////////////////////////////////////////////////////////////////////////////
// @Author	<student name>
// @Date		<date>  
//
// Function:
// multitap_echo
//
// @param - input - Buffer with input samples
//		  - output - Buffer with output samples
//		  - echo_state - Control state structure
//		  - mode - selected output mode
//
// @return - nothing
// Comment: Apply multichannel processing to input samples
//
// E-mail:	<email>
//
/////////////////////////////////////////////////////////////////////////////////
void processing(double input[][BLOCK_SIZE], double output[][BLOCK_SIZE], MultitapEcho** mEchos, int mode)
{
	//Apply multitap echo to left channel
	multitap_echo(input[L], output[L], BLOCK_SIZE, &mEchos[L]->echoState);

	//Apply multitap echo to right channel
	multitap_echo(input[R], output[R], BLOCK_SIZE, &mEchos[R]->echoState);

	if (mode == OM_3_0_0)
	{
		double gainL = 0.5;
		double gainR = 0.5;
		//Mix left and right channels and output result to center channel
		for (int i = 0; i < BLOCK_SIZE; i++)
		{
			output[C][i] = gainL * output[L][i]
				+ gainR * output[R][i];
		}
	}

}

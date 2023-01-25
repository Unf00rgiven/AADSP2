
#include "multitap_echo.h"
#include "processing.h"


#define MAX_NUM_OUT_CH		3
#define MAX_NUM_IN_CH		2
#define MAX_NUM_OUT_MODE	2 

// Index of channels
#define L		0
#define R		1
#define C       2

static const DSPint outputChMask[MAX_NUM_OUT_MODE][MAX_NUM_OUT_CH] =
{
	//L  R  C
	{ 1, 1, 0 },	//2.0.0 (stereo) 
	{ 1, 1, 1 }		//3.0.0 (stereo + center)
};

DSPint get_out_num_ch(DSPint mode)
{
	DSPint ret = 0;

	for (DSPint i = 0; i < MAX_NUM_OUT_CH; i++)
	{
		ret += outputChMask[mode][i];
	}

	return ret;
}

DSPint is_channel_active(DSPint mode, DSPint channel)
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
void processing(DSPfract input[][BLOCK_SIZE], DSPfract output[][BLOCK_SIZE], MultitapEcho** mEchos, int mode)
{
	//Apply multitap echo to left channel
	multitap_echo(input[L], output[L], &mEchos[L]->echoState);

	//Apply multitap echo to right channel
	multitap_echo(input[R], output[R], &mEchos[R]->echoState);

	if (mode == OM_3_0_0)
	{
		DSPfract gainL = FRACT_NUM(0.5);
		DSPfract gainR = FRACT_NUM(0.5);
		DSPfract* outputL = *(output + L);
		DSPfract* outputR = *(output + R);
		DSPfract* outputC = *(output + C);
		//Mix left and right channels and output result to center channel
		for (DSPint i = 0; i < BLOCK_SIZE; i++)
		{
			*outputC = gainL * *outputL + gainR * *outputR;
			outputC++;
			outputL++;
			outputR++;
		}
	}

}

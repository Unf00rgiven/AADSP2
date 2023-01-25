#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "WAVheader.h"
#include "common.h"

using namespace std;

// IO Buffers
static DSPfract sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

// Default variable gains
static DSPfract variablesGain[DEFAULT_OUTPUT_NUM_CHANNELS][INPUT_NUM_CHANNELS] = {{MINUS_4DB, PLUS_4DB }, { MINUS_4DB, PLUS_4DB }, { MINUS_4DB, PLUS_4DB }, { MINUS_4DB, PLUS_4DB }, { MINUS_4DB, PLUS_4DB }};


// Intermediate gained values
static DSPaccum gained[INPUT_NUM_CHANNELS];
static DSPfract out[DEFAULT_OUTPUT_NUM_CHANNELS];

// Filter buffers
static DSPfract history[DEFAULT_OUTPUT_NUM_CHANNELS][FILTERLEN];

DSPfract* pStateL = *(history + LEFT_CH);
DSPfract* pStateR = *(history + RIGHT_CH);
DSPfract* pStateLS = *(history + LS_CH);
DSPfract* pStateRS = *(history + RS_CH);
DSPfract* pStateC = *(history + C_CH);

void FilterInit()
{
	DSPfract* historyPtr1 = *(history + LEFT_CH);
	DSPfract* historyPtr2 = *(history + RIGHT_CH);
	DSPfract* historyPtr3 = *(history + LS_CH);
	DSPfract* historyPtr4 = *(history + RS_CH);
	DSPfract* historyPtr5 = *(history + C_CH);

	for (int j = 0; j < FILTERLEN; j++)
	{
		*historyPtr1 = FRACT_NUM(0.0);
		*historyPtr2 = FRACT_NUM(0.0);
		*historyPtr3 = FRACT_NUM(0.0);
		*historyPtr4 = FRACT_NUM(0.0);
		*historyPtr5 = FRACT_NUM(0.0);

		historyPtr1++;
		historyPtr2++;
		historyPtr3++;
		historyPtr4++;
		historyPtr5++;
	}
	*pStateL = FRACT_NUM(0.0);
	*pStateR = FRACT_NUM(0.0);
	*pStateLS = FRACT_NUM(0.0);
	*pStateRS = FRACT_NUM(0.0);
	*pStateC = FRACT_NUM(0.0);
}

DSPfract filterCoeff[FILTERLEN]{
	FRACT_NUM(-0.00032348877629069159),
	FRACT_NUM(0.00061391313136107599),
	FRACT_NUM(-0.00098367441013966672),
	FRACT_NUM(0.00135993640821725180),
	FRACT_NUM(-0.00157105555507173080),
	FRACT_NUM(0.00129463985232006930),
	FRACT_NUM(-0.00000265479966322842),
	FRACT_NUM(-0.00307975028120704550),
	FRACT_NUM(0.00897229562606529850),
	FRACT_NUM(-0.01884974934416482400),
	FRACT_NUM(0.03380441697827836500),
	FRACT_NUM(-0.05439224107032499700),
	FRACT_NUM(0.07990079675574889000),
	FRACT_NUM(-0.10731420700376081000),
	FRACT_NUM(0.13003365984943829000),
	FRACT_NUM(0.86107432527838756000),
	FRACT_NUM(0.13003365984943829000),
	FRACT_NUM(-0.10731420700376081000),
	FRACT_NUM(0.07990079675574889000),
	FRACT_NUM(-0.05439224107032499700),
	FRACT_NUM(0.03380441697827836500),
	FRACT_NUM(-0.01884974934416482400),
	FRACT_NUM(0.00897229562606529850),
	FRACT_NUM(-0.00307975028120704550),
	FRACT_NUM(-0.00000265479966322842),
	FRACT_NUM(0.00129463985232006930),
	FRACT_NUM(-0.00157105555507173080),
	FRACT_NUM(0.00135993640821725180),
	FRACT_NUM(-0.00098367441013966672),
	FRACT_NUM(0.00061391313136107599),
	FRACT_NUM(-0.00032348877629069159)
};

DSPfract fir_circular(DSPfract input, DSPfract **p_state, DSPfract *state_start, DSPfract *state_end)
{
	DSPint i;
	DSPfract* state = *p_state;
	DSPaccum ret_val;

	DSPfract* ptrCoeffs = (filterCoeff + FILTERLEN - 1);
	/* copy the filter's state to a local */

	/* store input at the beginning of the delay line */
	*state = input;					//history[state] = input;
	if (++state >= state_end)
	{								/* incr state and check for wrap */
		state = state_start;
	}

	/* calc FIR and shift data */
	ret_val = FRACT_NUM(0.0);
	for (i = FILTERLEN - 1; i >= 0; i--)
	{
		ret_val += *ptrCoeffs * *state;
		if (++state >= state_end)	/* incr state and check for wrap */
		{
			state = state_start;
		}
		ptrCoeffs--;
	}

	*p_state = state;               /* return new state to caller */

	return ret_val;
}


DSPfract saturation(DSPfract input)
{
	DSPaccum inValue = input;

	if (input > (DSPaccum)THRESHOLD)
	{
		return THRESHOLD;
	}
	if (input < (DSPaccum)-THRESHOLD)
	{
		return -THRESHOLD;
	}

	return input;
}


void Processing(DSPfract pIn[][BLOCK_SIZE], DSPfract pOut[][BLOCK_SIZE], DSPint LPFenable)
{
	DSPint i;

	// In samples
	DSPfract* ptrInL = *(pIn + LEFT_CH);
	DSPfract* ptrInR = *(pIn + RIGHT_CH);

	// Out pointers
	DSPfract* ptrOutL = *(pOut + LEFT_CH);
	DSPfract* ptrOutR = *(pOut + RIGHT_CH);
	DSPfract* ptrOutLS = *(pOut + LS_CH);
	DSPfract* ptrOutRS = *(pOut + RS_CH);
	DSPfract* ptrOutC = *(pOut + C_CH);

	for (i = 0; i < BLOCK_SIZE; i++)
	{	
		// Left Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + LEFT_CH))));				// L input * L gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + LEFT_CH))));				// R input * -L scaled gain
		*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
		*(out + LEFT_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);							// L channel = gained L + gained R

		// Right Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + RIGHT_CH))));				// L input * R gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + RIGHT_CH))));				// R input * -R scaled gain
		*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
		*(out + RIGHT_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);							// R channel = gained L + gained R

		// Ls Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + LS_CH))));					// L input * Ls gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + LS_CH))));					// R input * -Ls scaled gain
		*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
		*(out + LS_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);							// Ls channel = gained L + gained R
		
		// Rs Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + RS_CH))));					// L input * Rs gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + RS_CH))));					// R input * -Rs scaled gain
		*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
		*(out + RS_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);							// Rs channel = gained L + gained R

		// Center Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + C_CH))));					// L input * C gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + C_CH))));					// R input * -C scaled gain
		*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
		*(out + C_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);								// C channel = gained L + gained R
	
		// Direct output without filtering, SRS saturation after +
		*ptrOutL = saturation(*(out + LEFT_CH));
		*ptrOutR = saturation(*(out + RIGHT_CH));
		*ptrOutLS = saturation(*(out + LS_CH));
		*ptrOutRS = saturation(*(out + RS_CH));
		*ptrOutC = saturation(*(out + C_CH));

		// Filter processing
		if (LPFenable == 1)
		{
			*ptrOutL = fir_circular(*ptrOutL, &pStateL, *(history + LEFT_CH), *(history + LEFT_CH) + FILTERLEN);
			*ptrOutR = fir_circular(*ptrOutR, &pStateR, *(history + RIGHT_CH), *(history + RIGHT_CH) + FILTERLEN);
			*ptrOutLS = fir_circular(*ptrOutLS, &pStateLS, *(history + LS_CH), *(history + LS_CH) + FILTERLEN);
			*ptrOutRS = fir_circular(*ptrOutRS, &pStateRS, *(history + RS_CH), *(history + RS_CH) + FILTERLEN);
			*ptrOutC = fir_circular(*ptrOutC, &pStateC, *(history + C_CH), *(history + C_CH) + FILTERLEN);
		}
		
		// Saturiation before writing in output
		*ptrOutL = saturation(*ptrOutL);
		*ptrOutR = saturation(*ptrOutR);
		*ptrOutLS = saturation(*ptrOutLS);
		*ptrOutRS = saturation(*ptrOutRS);
		*ptrOutC = saturation(*ptrOutC);

		ptrInL++;
		ptrInR++;
		ptrOutL++;
		ptrOutR++;
		ptrOutLS++;
		ptrOutRS++;
		ptrOutC++;

	}
	
}


DSPfract dBtoLinear(double value)
{
	// Converting +dB to linear, double -> fract
	return (pow(10, value / 20) / 16);
	
}

/////////////////////////////////////////////////////////////////////////////////
// @Author	Ognjen Stojisavljevic RA 155 2019
// @Date	November 2022 
//
// Function:
// main
//
// @param - argv[1] - Input file name
//        - argv[2] - Output file name
//		  - argv[3] - LPF enable
//		  - argv[4] - GL
//		  - argv[5] - GR
//		  - argv[6] - GLs
//		  - argv[7] - GRs
//		  - argv[8] - GC
//		  - argv[9] - mode
//
// @return - nothing
// Comment: main routine of a program
//
// E-mail:	stojisavljevic00@gmail.com
//
// model0 agrs: D:\TestStreams\funky_sample.wav D:\TestStreams\funky_sample_model0.wav 1 -4 -4 -4 -4 -4 1
/////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	FILE* wav_in = NULL;
	FILE* wav_out = NULL;
	char WavInputName[256];
	char WavOutputName[256];
	WAV_HEADER inputWAVhdr, outputWAVhdr;
	double positiveGain;
	// Default mode and enable
	int mode = 1;
	int LPFenable = 1; 
	
	// Reseting filter
	FilterInit();

	// Must have 3 args, exe, input and output file 
	// Other arguments are default or arg[i]
	if (argc < 3)
	{
		cout << "Invalid arguments\n" << endl;
		return 0;
	}

	// Get enable, gains, mode
	if (argc >= 4)
	{
		LPFenable = atoi(argv[3]);
	}

	// db to linear conversion
	if (argc >= 5)
	{
		variablesGain[LEFT_CH][0] = pow(10, atof(argv[4])/20);
		positiveGain = -atof(argv[4]);
		variablesGain[LEFT_CH][1] = dBtoLinear(positiveGain);

	}

	if (argc >= 6)
	{
		variablesGain[RIGHT_CH][0] = pow(10, atof(argv[5]) / 20);
		positiveGain = -atof(argv[5]);
		variablesGain[RIGHT_CH][1] = dBtoLinear(positiveGain);
	}

	if (argc >= 7)
	{
		variablesGain[LS_CH][0] = pow(10, atof(argv[6]) / 20);
		positiveGain = -atof(argv[6]);
		variablesGain[LS_CH][1] = dBtoLinear(positiveGain);
	}
	
	if (argc >= 8)
	{
		variablesGain[RS_CH][0] = pow(10, atof(argv[7]) / 20);
		positiveGain = -atof(argv[7]);
		variablesGain[RS_CH][1] = dBtoLinear(positiveGain);
	}

	if (argc >= 9)
	{
		variablesGain[C_CH][0] = pow(10, atof(argv[8]) / 20);
		positiveGain = -atof(argv[8]);
		variablesGain[C_CH][1] = dBtoLinear(positiveGain);
	}
	if (argc >= 10)
	{
		mode = atoi(argv[9]);
	}
	
	
	// Init channel buffers
	for (DSPint i = 0; i < MAX_NUM_CHANNEL; i++)
		for (DSPint j = 0; j < BLOCK_SIZE; j++)
			sampleBuffer[i][j] = FRACT_NUM(0.0);

	// Open input and output wav files
	//-------------------------------------------------
	strcpy(WavInputName, argv[1]);
	wav_in = OpenWavFileForRead(WavInputName, "rb");
	strcpy(WavOutputName, argv[2]);
	wav_out = OpenWavFileForRead(WavOutputName, "wb");
	//-------------------------------------------------


	// Read input wav header
	//-------------------------------------------------
	ReadWavHeader(wav_in, inputWAVhdr);
	//-------------------------------------------------

	// Set up output WAV header
	//-------------------------------------------------	
	outputWAVhdr = inputWAVhdr;
	if(mode == 1) outputWAVhdr.fmt.NumChannels = DEFAULT_OUTPUT_NUM_CHANNELS; // change number of channels
	else outputWAVhdr.fmt.NumChannels = OUTPUT_NUM_CHANNELS_2;

	int oneChannelSubChunk2Size = inputWAVhdr.data.SubChunk2Size / inputWAVhdr.fmt.NumChannels;
	int oneChannelByteRate = inputWAVhdr.fmt.ByteRate / inputWAVhdr.fmt.NumChannels;
	int oneChannelBlockAlign = inputWAVhdr.fmt.BlockAlign / inputWAVhdr.fmt.NumChannels;

	outputWAVhdr.data.SubChunk2Size = oneChannelSubChunk2Size * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.ByteRate = oneChannelByteRate * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.BlockAlign = oneChannelBlockAlign * outputWAVhdr.fmt.NumChannels;


	// Write output WAV header to file
	//-------------------------------------------------
	WriteWavHeader(wav_out, outputWAVhdr);


	// Processing loop
	//-------------------------------------------------	
	{
		int sample;
		int BytesPerSample = inputWAVhdr.fmt.BitsPerSample / 8;
		const double SAMPLE_SCALE = -(double)(1 << 31);		//2^31
		int iNumSamples = inputWAVhdr.data.SubChunk2Size / (inputWAVhdr.fmt.NumChannels * inputWAVhdr.fmt.BitsPerSample / 8);

		// exact file length should be handled correctly...
		for (int i = 0; i < iNumSamples / BLOCK_SIZE; i++)
		{
			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				for (int k = 0; k < inputWAVhdr.fmt.NumChannels; k++)
				{
					sample = 0; //debug
					fread(&sample, BytesPerSample, 1, wav_in);
					sample = sample << (32 - inputWAVhdr.fmt.BitsPerSample); // force signextend
					sampleBuffer[k][j] = sample / SAMPLE_SCALE;				// scale sample to 1.0/-1.0 range		
				}
			}

			Processing(sampleBuffer, sampleBuffer, LPFenable);

			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				for (int k = 0; k < outputWAVhdr.fmt.NumChannels; k++)
				{
					sample = sampleBuffer[k][j].toLong();	// crude, non-rounding 			
					sample = sample >> (32 - inputWAVhdr.fmt.BitsPerSample);
					fwrite(&sample, outputWAVhdr.fmt.BitsPerSample / 8, 1, wav_out);
				}
			}
		}
	}



	// Close files
	//-------------------------------------------------	
	fclose(wav_in);
	fclose(wav_out);
	//-------------------------------------------------	

	return 0;
}
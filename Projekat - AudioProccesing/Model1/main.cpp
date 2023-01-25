#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <math.h>
#include "WAVheader.h"

using namespace std;

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
#define PLUS_4DB 1.58489

// Saturation threshold
#define THRESHOLD 0.999
#define FILTERLEN 31

// IO Buffers
static double sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

// Default variable gains
static double variablesGain[DEFAULT_OUTPUT_NUM_CHANNELS][INPUT_NUM_CHANNELS] = { { MINUS_4DB, PLUS_4DB },{ MINUS_4DB, PLUS_4DB },{ MINUS_4DB, PLUS_4DB },{ MINUS_4DB, PLUS_4DB },{ MINUS_4DB, PLUS_4DB } };

// Intermediate gained values
static double gained[INPUT_NUM_CHANNELS];
static double out[DEFAULT_OUTPUT_NUM_CHANNELS];

// Filter buffers
static double history[DEFAULT_OUTPUT_NUM_CHANNELS][FILTERLEN];

double* pStateL = *(history + LEFT_CH);
double* pStateR = *(history + RIGHT_CH);
double* pStateLS = *(history + LS_CH);
double* pStateRS = *(history + RS_CH);
double* pStateC = *(history + C_CH);

void FilterInit()
{
	double* historyPtr1 = *history;
	double* historyPtr2 = historyPtr1++;
	double* historyPtr3 = historyPtr2++;
	double* historyPtr4 = historyPtr3++;
	double* historyPtr5 = historyPtr4++;

	for (int j = 0; j < FILTERLEN; j++)
	{
		*historyPtr1 = 0;
		*historyPtr2 = 0;
		*historyPtr3 = 0;
		*historyPtr4 = 0;
		*historyPtr5 = 0;

		historyPtr1++;
		historyPtr2++;
		historyPtr3++;
		historyPtr4++;
		historyPtr5++;
	}

	*pStateL = 0;
	*pStateR = 0;
	*pStateLS = 0;
	*pStateRS = 0;
	*pStateC = 0;

}

double filterCoeff[FILTERLEN]{
	-0.00032348877629069159,
	0.00061391313136107599,
	-0.00098367441013966672,
	0.00135993640821725180,
	-0.00157105555507173080,
	0.00129463985232006930,
	-0.00000265479966322842,
	-0.00307975028120704550,
	0.00897229562606529850,
	-0.01884974934416482400,
	0.03380441697827836500,
	-0.05439224107032499700,
	0.07990079675574889000,
	-0.10731420700376081000,
	0.13003365984943829000,
	0.86107432527838756000,
	0.13003365984943829000,
	-0.10731420700376081000,
	0.07990079675574889000,
	-0.05439224107032499700,
	0.03380441697827836500,
	-0.01884974934416482400,
	0.00897229562606529850,
	-0.00307975028120704550,
	-0.00000265479966322842,
	0.00129463985232006930,
	-0.00157105555507173080,
	0.00135993640821725180,
	-0.00098367441013966672,
	0.00061391313136107599,
	-0.00032348877629069159
};

double fir_circular(double input, double **p_state, double *state_start, double *state_end)
{
	int i;
	double *state = *p_state;
	double ret_val;

	double* ptrCoeffs = (filterCoeff + FILTERLEN - 1);
	/* copy the filter's state to a local */

	/* store input at the beginning of the delay line */
	*state = input;					//history[state] = input;
	if (++state >= state_end)
	{								/* incr state and check for wrap */
		state = state_start;
	}

	/* calc FIR and shift data */
	ret_val = 0;
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


double saturation(double input)
{
	double threshold = THRESHOLD;

	if (input > threshold)
	{
		return threshold;
	}
	if (input < -threshold)
	{
		return -threshold;
	}

	return input;
}


void Processing(double pIn[][BLOCK_SIZE], double pOut[][BLOCK_SIZE], int LPFenable)
{
	int i;

	// In samples
	double* ptrInL = *(pIn + LEFT_CH);
	double* ptrInR = *(pIn + RIGHT_CH);

	// Out pointers
	double* ptrOutL = *(pOut + LEFT_CH);
	double* ptrOutR = *(pOut + RIGHT_CH);
	double* ptrOutLS = *(pOut + LS_CH);
	double* ptrOutRS = *(pOut + RS_CH);
	double* ptrOutC = *(pOut + C_CH);

	for (i = 0; i < BLOCK_SIZE; i++)
	{
		// Left Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + LEFT_CH))));		// L input * L gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + LEFT_CH))));		// R input * -L gain
		*(out + LEFT_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);					// L channel = gained L + gained R

		// Right Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + RIGHT_CH))));		// L input * R gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + RIGHT_CH))));		// R input * -R gain
		*(out + RIGHT_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);					// R channel = gained L + gained R

		// Ls Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + LS_CH))));			// L input * Ls gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + LS_CH))));			// R input * -Ls gain
		*(out + LS_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);					// Ls channel = gained L + gained R

		// Rs Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + RS_CH))));			// L input * Rs gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + RS_CH))));			// R input * -Rs gain
		*(out + RS_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);					// Rs channel = gained L + gained R

		// Center Channel processing
		*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + C_CH))));			// L input * C gain
		*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + C_CH))));			// R input * -C gain
		*(out + C_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);						// C channel = gained L + gained R

		// Direct output withot filtering, SRS saturation after +
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

		// Saturation after filtering
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
	// Default mode and enable
	int mode = 1;
	int LPFenable = 1;

	// Reseting filter history
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
		variablesGain[LEFT_CH][0] = pow(10, atof(argv[4]) / 20);
		variablesGain[LEFT_CH][1] = pow(10, -atof(argv[4]) / 20);

	}

	if (argc >= 6)
	{
		variablesGain[RIGHT_CH][0] = pow(10, atof(argv[5]) / 20);
		variablesGain[RIGHT_CH][1] = pow(10, -atof(argv[5]) / 20);
	}

	if (argc >= 7)
	{
		variablesGain[LS_CH][0] = pow(10, atof(argv[6]) / 20);
		variablesGain[LS_CH][1] = pow(10, -atof(argv[6]) / 20);
	}

	if (argc >= 8)
	{
		variablesGain[RS_CH][0] = pow(10, atof(argv[7]) / 20);
		variablesGain[RS_CH][1] = pow(10, -atof(argv[7]) / 20);
	}

	if (argc >= 9)
	{
		variablesGain[C_CH][0] = pow(10, atof(argv[8]) / 20);
		variablesGain[C_CH][1] = pow(10, -atof(argv[8]) / 20);
	}
	if (argc >= 10)
	{
		mode = atoi(argv[9]);
	}


	// Init channel buffers
	for (int i = 0; i < MAX_NUM_CHANNEL; i++)
		memset(&sampleBuffer[i], 0, BLOCK_SIZE);

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
	if (mode == 1) outputWAVhdr.fmt.NumChannels = DEFAULT_OUTPUT_NUM_CHANNELS; // change number of channels
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
					sample = sampleBuffer[k][j] * SAMPLE_SCALE;	// crude, non-rounding 			
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
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

// Filter buffers
static double history[DEFAULT_OUTPUT_NUM_CHANNELS][FILTERLEN];
static unsigned int pState[DEFAULT_OUTPUT_NUM_CHANNELS];

void FilterInit()
{
	
	for (int j = 0; j < DEFAULT_OUTPUT_NUM_CHANNELS; j++)
	{
		for (int i = 0; i < FILTERLEN; i++)
		{
			history[j][i] = 0;
		}
	}

	for (int i = 0; i < DEFAULT_OUTPUT_NUM_CHANNELS; i++)
	{
		pState[i] = 0;
	}
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

double fir_circular(double input, double *coeffs, double *history, unsigned int n_coeff, unsigned int *p_state)
{
	int i;
	unsigned int state;
	double ret_val;

	state = *p_state;               /* copy the filter's state to a local */


									/* store input at the beginning of the delay line */
	history[state] = input;
	if (++state >= n_coeff)
	{         /* incr state and check for wrap */
		state = 0;
	}

	/* calc FIR and shift data */
	ret_val = 0;
	for (i = n_coeff - 1; i >= 0; i--)
	{
		ret_val += coeffs[i] * history[state];
		if (++state >= n_coeff) /* incr state and check for wrap */
		{
			state = 0;
		}
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


void Processing(double pIn[][BLOCK_SIZE], double pOut[][BLOCK_SIZE], double variableGains[][2], int LPFenable)
{
	int i;
	double gained[INPUT_NUM_CHANNELS];
	double out[DEFAULT_OUTPUT_NUM_CHANNELS];

	for (i = 0; i < BLOCK_SIZE; i++)
	{
		// Left Channel processing
		gained[LEFT_CH] = pIn[LEFT_CH][i] * variableGains[LEFT_CH][0];		// L input * L gain
		gained[RIGHT_CH] = pIn[RIGHT_CH][i] * variableGains[LEFT_CH][1];	// R input * L gain
		out[LEFT_CH] = gained[LEFT_CH] + gained[RIGHT_CH];					// L channel = gained L + gained R

		// Right Channel processing
		gained[LEFT_CH] = pIn[LEFT_CH][i] * variableGains[RIGHT_CH][0];		// L input * R gain
		gained[RIGHT_CH] = pIn[RIGHT_CH][i] * variableGains[RIGHT_CH][1];	// R input * R gain
		out[RIGHT_CH] = gained[LEFT_CH] + gained[RIGHT_CH];					// R channel = gained L + gained R

		// Ls Channel processing
		gained[LEFT_CH] = pIn[LEFT_CH][i] * variableGains[LS_CH][0];		// L input * Ls gain
		gained[RIGHT_CH] = pIn[RIGHT_CH][i] * variableGains[LS_CH][1];		// R input * Ls gain
		out[LS_CH] = gained[LEFT_CH] + gained[RIGHT_CH];					// Ls channel = gained L + gained R

		// Rs Channel processing
		gained[LEFT_CH] = pIn[LEFT_CH][i] * variableGains[RS_CH][0];		// L input * Rs gain
		gained[RIGHT_CH] = pIn[RIGHT_CH][i] * variableGains[RS_CH][1];		// R input * Rs gain
		out[RS_CH] = gained[LEFT_CH] + gained[RIGHT_CH];					// Rs channel = gained L + gained R

		// Center Channel processing
		gained[LEFT_CH] = pIn[LEFT_CH][i] * variableGains[C_CH][0];			// L input * C gain
		gained[RIGHT_CH] = pIn[RIGHT_CH][i] * variableGains[C_CH][1];		// R input * C gain
		out[C_CH] = gained[LEFT_CH] + gained[RIGHT_CH];						// C channel = gained L + gained R

		// Without filtering, direct output, SRS saturation after +
		pOut[LEFT_CH][i] = saturation(out[LEFT_CH]);
		pOut[RIGHT_CH][i] = saturation(out[RIGHT_CH]);
		pOut[LS_CH][i] = saturation(out[LS_CH]);
		pOut[RS_CH][i] = saturation(out[RS_CH]);
		pOut[C_CH][i] = saturation(out[C_CH]);

		// Filter processing
		if (LPFenable == 1)
		{
			pOut[LEFT_CH][i] = fir_circular(pOut[LEFT_CH][i], filterCoeff, history[LEFT_CH], FILTERLEN, &pState[LEFT_CH]);
			pOut[RIGHT_CH][i] = fir_circular(pOut[RIGHT_CH][i], filterCoeff, history[RIGHT_CH], FILTERLEN, &pState[RIGHT_CH]);
			pOut[LS_CH][i] = fir_circular(pOut[LS_CH][i], filterCoeff, history[LS_CH], FILTERLEN, &pState[LS_CH]);
			pOut[RS_CH][i] = fir_circular(pOut[RS_CH][i], filterCoeff, history[RS_CH], FILTERLEN, &pState[RS_CH]);
			pOut[C_CH][i] = fir_circular(pOut[C_CH][i], filterCoeff, history[C_CH], FILTERLEN, &pState[C_CH]);
		}
		// Saturation after filtering
		pOut[LEFT_CH][i] = saturation(pOut[LEFT_CH][i]);
		pOut[RIGHT_CH][i] = saturation(pOut[RIGHT_CH][i]);
		pOut[LS_CH][i] = saturation(pOut[LS_CH][i]);
		pOut[RS_CH][i] = saturation(pOut[RS_CH][i]);
		pOut[C_CH][i] = saturation(pOut[C_CH][i]);
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
// model0 agrs: input output 1enabled -4db -4db -4db -4db -4db 1(5 channel)
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

			Processing(sampleBuffer, sampleBuffer, variablesGain, LPFenable);


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
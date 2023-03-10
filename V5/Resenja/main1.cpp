
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "WAVheader.h"

#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8

// Number of channels
#define INPUT_NUM_CHANNELS 2
#define OUTPUT_NUM_CHANNELS 3

// Channel IDs. 
// Should input and output channel IDs be separated?
#define LEFT_CH 0
#define RIGHT_CH 1
#define CENTER_CH 2

// Gain linear values. 
#define PLUS_6DB 1.99526
#define MINUS_2DB 0.794328
#define MINUS_4DB 0.630957
#define MINUS_12DB 0.251189 

// IO Buffers
static double sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

// Processing related variables
static double preGain;
static double postGain;
static double variablesGain[INPUT_NUM_CHANNELS];
static double limiterThreshold = 0.999;

void initGainProcessing(double preGainValue, double* defaultVariablesGain, double postGainValue)
{
	preGain = preGainValue;
	for (int i = 0; i < INPUT_NUM_CHANNELS; i++)
	{
		variablesGain[i] = defaultVariablesGain[i];
	}
	postGain = postGainValue;
}

double saturation(double in, double threshold)
{
	// Simple limiter since we know that pre-Gain adds 6dB
	if (in > threshold)
	{
		return fmin(in, threshold);
	}
	else if (in < -threshold)
	{
		return fmax(in, -threshold);
	}

	return in;
}

void gainProcessing(double pIn[][BLOCK_SIZE], double pOut[][BLOCK_SIZE], const double LchGain, const double RchGain, double* variableGains, const double CchGain, int noInputChannels, int nSamples)
{
	double* samplePtrInL = *pIn;
	double* samplePtrInR = *(pIn + RIGHT_CH);
	double* samplePtrOutL = *pOut;
	double* samplePtrOutR = *(pOut + RIGHT_CH);
	double* CenterSamplePtrOut = *(pOut + CENTER_CH);
	double* variableGainsPtr = variableGains;


	for (int i = 0; i < nSamples; i++)
	{
		*samplePtrInL = (*samplePtrInL) * LchGain;
		*samplePtrInR = (*samplePtrInR) * RchGain;

		*samplePtrOutL = saturation((*samplePtrInL) * (*variableGainsPtr), limiterThreshold);
		*samplePtrOutR = saturation((*samplePtrInR) * (*(variableGainsPtr + RIGHT_CH)), limiterThreshold);
		*CenterSamplePtrOut += (*samplePtrOutL) + (*samplePtrOutR);
		*CenterSamplePtrOut *= CchGain;


		samplePtrInL++;
		samplePtrInR++;
		samplePtrOutL++;
		samplePtrOutR++;
		CenterSamplePtrOut++;

	}


	//for (int j = 0; j < nsamples; j++)
	//{
	//	pin[left_ch][j] = pin[left_ch][j] * lchgain;
	//	pin[right_ch][j] = pin[right_ch][j] * rchgain;
	//	pout[left_ch][j] = saturation(pin[left_ch][j] * variablegains[left_ch], limiterthreshold);
	//	pout[right_ch][j] = saturation(pin[right_ch][j] * variablegains[right_ch], limiterthreshold);
	//	pout[center_ch][j] += pout[left_ch][j] + pout[right_ch][j];
	//	pout[center_ch][j] *= cchgain;
	//}



}

/////////////////////////////////////////////////////////////////////////////////
// @Author	<student name>
// @Date		<date>  
//
// Function:
// main
//
// @param - argv[0] - Input file name
//        - argv[1] - Output file name
// @return - nothing
// Comment: main routine of a program
//
// E-mail:	<email>
//
/////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	FILE* wav_in = NULL;
	FILE* wav_out = NULL;
	char WavInputName[256];
	char WavOutputName[256];
	WAV_HEADER inputWAVhdr, outputWAVhdr;
	double defaultVariablesGain[INPUT_NUM_CHANNELS] = { MINUS_4DB , MINUS_2DB }; // -2dB, -4dB

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
	outputWAVhdr.fmt.NumChannels = OUTPUT_NUM_CHANNELS; // change number of channels

	int oneChannelSubChunk2Size = inputWAVhdr.data.SubChunk2Size / inputWAVhdr.fmt.NumChannels;
	int oneChannelByteRate = inputWAVhdr.fmt.ByteRate / inputWAVhdr.fmt.NumChannels;
	int oneChannelBlockAlign = inputWAVhdr.fmt.BlockAlign / inputWAVhdr.fmt.NumChannels;

	outputWAVhdr.data.SubChunk2Size = oneChannelSubChunk2Size * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.ByteRate = oneChannelByteRate * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.BlockAlign = oneChannelBlockAlign * outputWAVhdr.fmt.NumChannels;


	// Write output WAV header to file
	//-------------------------------------------------
	WriteWavHeader(wav_out, outputWAVhdr);

	initGainProcessing(PLUS_6DB, defaultVariablesGain, MINUS_12DB);

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
			// enable , mute , and gain coeffs
			if (atoi(argv[3]) == 1) {
				if (atoi(argv[4]) == 1)
				{
					variablesGain[0] = 0;
					variablesGain[1] = 0;
				}
				else
				{
					variablesGain[0] = atof(argv[5]);
					variablesGain[1] = atof(argv[6]);

				}
				gainProcessing(sampleBuffer, sampleBuffer, preGain, preGain, variablesGain, postGain, INPUT_NUM_CHANNELS, BLOCK_SIZE);
			}


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
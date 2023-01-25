
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

void gainProcessing(double pIn[][BLOCK_SIZE], double pOut[][BLOCK_SIZE], const double LchGain, const double RchGain, double* variableGains, const double CchGain, int noInputChannels, int nSamples, int mode)
{
	double preGains[INPUT_NUM_CHANNELS] = { LchGain, RchGain };
	double postGain = CchGain;

	//for (int i = 0; i < noinputchannels; i++)
	//{
	//	for (int j = 0; j < nsamples; j++)
	//	{
	//		// first stage, apply constant pre-gain
	//		pin[i][j] = pin[i][j] * pregains[i];
	//		// second stage, apply variable gain
	//		pout[i][j] = saturation(pin[i][j] * variablegains[i], limiterthreshold);
	//		// add processed sampled to the center output channel
	//		pout[center_ch][j] += pout[i][j];
	//		// apply center channel post-gain
	//		pout[center_ch][j] *= (postgain/2);
	//	}
	//}

	// TODO: remove upper implementation and implement processing for each channel indepenetnly 
	// (without outter noInputChannels loop, but only with inner nSamples loop)
	// (kick-out any unnecessary local variables and parameters)
	for (int j = 0; j < nSamples; j++)
	{
		pIn[LEFT_CH][j] = pIn[LEFT_CH][j] * LchGain;
		pIn[RIGHT_CH][j] = pIn[RIGHT_CH][j] * RchGain;
		pOut[LEFT_CH][j] = saturation(pIn[LEFT_CH][j] * variableGains[LEFT_CH], limiterThreshold);
		pOut[RIGHT_CH][j] = saturation(pIn[RIGHT_CH][j] * variableGains[RIGHT_CH], limiterThreshold);
		if (mode == 0)		// ovo nije potrebno jer smo svakako upisali u waf koliko ima izlaznih kanala
		{
			pOut[CENTER_CH][j] += pOut[LEFT_CH][j] + pOut[RIGHT_CH][j];
			pOut[CENTER_CH][j] *= CchGain;
		}
	}

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
	int mode = 0;

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
	mode = atoi(argv[7]);
	if (mode == 0) outputWAVhdr.fmt.NumChannels = OUTPUT_NUM_CHANNELS; // change number of channels;
	else if (mode == 1) outputWAVhdr.fmt.NumChannels = 2;

	

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
				gainProcessing(sampleBuffer, sampleBuffer, preGain, preGain, variablesGain, postGain, INPUT_NUM_CHANNELS, BLOCK_SIZE, mode);
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
#include <stdio.h>
#include <dsplib\wavefile.h>
#include <stdfix.h>
#include <string.h>
#include "common.h"

__memY DSPfract sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

// Processing related variables
DSPfract preGain = PLUS_6DB;
DSPfract postGain = MINUS_12DB;
DSPfract variablesGain[INPUT_NUM_CHANNELS] = {MINUS_4DB, MINUS_2DB};
DSPfract limiterThreshold = FRACT_NUM(0.999);

//#ifdef GAINPROC_ASM
//extern void gainProccesing(__memY DSPfract pIn[][BLOCK_SIZE], __memY DSPfract pOut[][BLOCK_SIZE]);
//#else

void initGainProcessing(DSPfract preGainValue, DSPfract* defaultVariablesGain, DSPfract postGainValue)
{
	DSPint i = 0;
	preGain = preGainValue;

	for (i = 0; i < INPUT_NUM_CHANNELS; i++)
	{
		variablesGain[i] = defaultVariablesGain[i];
	}
	postGain = postGainValue;
}

DSPfract saturation(DSPfract in)
{
	DSPaccum value = in;
	// Simple limiter since we know that pre-Gain adds 6dB
	if (value > (DSPaccum)limiterThreshold)
	{
		return limiterThreshold;
	}
	else if (value < (DSPaccum)-limiterThreshold)
	{
		return -limiterThreshold;
	}

	return value;
}


void gainProcessing(__memY DSPfract pIn[][BLOCK_SIZE],__memY DSPfract pOut[][BLOCK_SIZE])
{
	__memY DSPfract* samplePtrInL = *(pIn + LEFT_CH);
	__memY DSPfract* samplePtrInR = *(pIn + RIGHT_CH);
	__memY DSPfract* samplePtrOutL = *(pOut + LEFT_CH);
	__memY DSPfract* samplePtrOutR = *(pOut + RIGHT_CH);
	__memY DSPfract* centerSamplePtr = *(pOut + CENTER_CH);
	DSPfract* variableGainsPtr = variablesGain;
	DSPint i;
	DSPaccum scaledSampleL;
	DSPaccum scaledSampleR;
	DSPaccum scaledSampleCenter;

	for (i = 0; i < BLOCK_SIZE; i++)
	{
		// first stage, apply constant pre-Gain
		*samplePtrInL = (*samplePtrInL) * preGain;
		*samplePtrInR = (*samplePtrInR) * preGain;

		// second stage, apply variable gain
		*samplePtrOutL = (*samplePtrInL) * (*variableGainsPtr);
		*samplePtrOutR = (*samplePtrInR) * (*(variableGainsPtr + RIGHT_CH));

		// Rescalling
		scaledSampleL = (*samplePtrOutL) << 1;
		scaledSampleR = (*samplePtrOutR) << 1;

		scaledSampleL = saturation(scaledSampleL);
		scaledSampleR = saturation(scaledSampleR);
		*samplePtrOutL = scaledSampleL;
		*samplePtrOutR = scaledSampleR;

		scaledSampleCenter = *centerSamplePtr;
		// add processed samples to the center output channel
		scaledSampleCenter = scaledSampleCenter + scaledSampleL + scaledSampleR;
		scaledSampleCenter = scaledSampleCenter >> 2;
		*centerSamplePtr = scaledSampleCenter;
		// apply center channel post-Gain
		*centerSamplePtr = (*centerSamplePtr) * postGain;
		*centerSamplePtr = (*centerSamplePtr) << 2;

		samplePtrInL++;
		samplePtrInR++;
		samplePtrOutL++;
		samplePtrOutR++;
		centerSamplePtr++;
	}


}
//#endif

int main(int argc, char *argv[])
 {
    WAVREAD_HANDLE *wav_in;
    WAVWRITE_HANDLE *wav_out;

	char WavInputName[256];
	char WavOutputName[256];

    DSPint inChannels;
    DSPint outChannels;
    DSPint bitsPerSample;
    DSPint sampleRate;
    DSPint iNumSamples;
    DSPint i;
    DSPint j;

	// Init channel buffers
	for(i=0; i<MAX_NUM_CHANNEL; i++)
		for(j=0; j<BLOCK_SIZE; j++)
			sampleBuffer[i][j] = FRACT_NUM(0.0);

	// Open input wav file
	//-------------------------------------------------
	strcpy(WavInputName,argv[0]);
	wav_in = cl_wavread_open(WavInputName);
	 if(wav_in == NULL)
    {
        printf("Error: Could not open wavefile.\n");
        return -1;
    }
	//-------------------------------------------------

	// Read input wav header
	//-------------------------------------------------
	inChannels = cl_wavread_getnchannels(wav_in);
    bitsPerSample = cl_wavread_bits_per_sample(wav_in);
    sampleRate = cl_wavread_frame_rate(wav_in);
    iNumSamples =  cl_wavread_number_of_frames(wav_in);
	//-------------------------------------------------

	// Open output wav file
	//-------------------------------------------------
	strcpy(WavOutputName,argv[1]);
	outChannels = 3;
	wav_out = cl_wavwrite_open(WavOutputName, bitsPerSample, outChannels, sampleRate);
	if(!wav_out)
    {
        printf("Error: Could not open wavefile.\n");
        return -1;
    }
	//-------------------------------------------------

	// Processing loop
	//-------------------------------------------------
    {
		int i;
		int j;
		int k;
		int sample;

		// exact file length should be handled correctly...
		for(i=0; i<iNumSamples/BLOCK_SIZE; i++)
		{
			for(j=0; j<BLOCK_SIZE; j++)
			{
				for(k=0; k<inChannels; k++)
				{
					sample = cl_wavread_recvsample(wav_in);
        			sampleBuffer[k][j] = rbits(sample);
				}
			}

			// pozvati processing funkciju ovde
			gainProcessing(sampleBuffer, sampleBuffer);

			for(j=0; j<BLOCK_SIZE; j++)
			{
				for(k=0; k<outChannels; k++)
				{
					sample = bitsr(sampleBuffer[k][j]);
					cl_wavwrite_sendsample(wav_out, sample);
				}
			}
		}
	}

	// Close files
	//-------------------------------------------------
    cl_wavread_close(wav_in);
    cl_wavwrite_close(wav_out);
	//-------------------------------------------------

    return 0;
 }

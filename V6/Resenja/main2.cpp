
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "C:\Users\Student\Desktop\RA 155-2019\AAiDSP\V5\VE-5\VE-50\WAVheader.h"
#include "C:\Users\Student\Desktop\RA 155-2019\AAiDSP\VE-6\common.h"


// IO Buffers
static DSPfract sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

// Processing related variables
static DSPfract preGain;
static DSPfract postGain;
static DSPfract variablesGain[INPUT_NUM_CHANNELS];
static DSPfract limiterThreshold = FRACT_NUM(0.999) ;

void initGainProcessing(DSPfract preGainValue, DSPfract* defaultVariablesGain, DSPfract postGainValue)
{
	preGain = preGainValue;
	for (DSPint i = 0; i < INPUT_NUM_CHANNELS; i++)
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

void gainProcessing(DSPfract pIn[][BLOCK_SIZE], DSPfract pOut[][BLOCK_SIZE])
{
	DSPfract* samplePtrInL = *(pIn + LEFT_CH);
	DSPfract* samplePtrInR = *(pIn + RIGHT_CH);
	DSPfract* samplePtrOutL = *(pOut + LEFT_CH);
	DSPfract* samplePtrOutR = *(pOut + RIGHT_CH);
	DSPfract* centerSamplePtr = *(pOut + CENTER_CH);
	DSPfract* variableGainsPtr = variablesGain;


	for (DSPint i = 0; i < BLOCK_SIZE; i++)
	{
		// first stage, apply constant pre-Gain
		*samplePtrInL = (*samplePtrInL) * preGain;
		*samplePtrInR = (*samplePtrInR) * preGain;

		// second stage, apply variable gain
		*samplePtrOutL = (*samplePtrInL) * (*variableGainsPtr);
		*samplePtrOutR = (*samplePtrInR) * (*(variableGainsPtr + RIGHT_CH));

		// Rescalling 
		DSPaccum scaledSampleL = (*samplePtrOutL) << 1;
		DSPaccum scaledSampleR = (*samplePtrOutR) << 1;
	
		scaledSampleL = saturation(scaledSampleL);
		scaledSampleR = saturation(scaledSampleR);
		*samplePtrOutL = scaledSampleL;
		*samplePtrOutR = scaledSampleR;

		DSPaccum scaledSampleCenter = *centerSamplePtr;
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
	DSPfract defaultVariablesGain[INPUT_NUM_CHANNELS] = { MINUS_4DB , MINUS_2DB }; // -2dB, -4dB

																				 // Init channel buffers
	for (int i = 0; i < MAX_NUM_CHANNEL; i++)
		for (int j = 0; j < BLOCK_SIZE; j++)
		{
			sampleBuffer[i][j] = FRACT_NUM(0.0);
		}

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
					variablesGain[0] = (DSPfract)0;
					variablesGain[1] = (DSPfract)0;
				}
				else
				{
					variablesGain[0] = atof(argv[5]);
					variablesGain[1] = atof(argv[6]);

				}
				gainProcessing(sampleBuffer, sampleBuffer);
			}


			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				for (int k = 0; k < outputWAVhdr.fmt.NumChannels; k++)
				{
					sample = sampleBuffer[k][j].toLong();
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
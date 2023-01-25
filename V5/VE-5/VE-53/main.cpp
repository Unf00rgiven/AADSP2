/////////////////////////////////////////////////////////////////////////////////
//
// @file main.cpp
//
// Module: multitapEcho
// Description:  Add multitap echo to input signal
// $Source: $
// $Revision: 1.0 $
// $Date: <date> $
// $Author: <student name> $
//
/////////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "multitap_echo.h"
#include "processing.h"
#include "WAVheader.h"

/////////////////////////////////////////////////////////////////////////////////
// IO buffers
/////////////////////////////////////////////////////////////////////////////////
double sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];
/////////////////////////////////////////////////////////////////////////////////

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

	//Set default mode
	int mode = OM_3_0_0;


	// Multitap delay state and initialization constants
	//-------------------------------------------------
	MultitapEcho mEcho[2];
	MultitapEcho* pmEcho[2] = { &mEcho[0], &mEcho[1] };
	const int initial_delay[N_TAP] = { 1024, 1536, 2560, 3072 };
	const double initial_gain[N_TAP] = { 0.75, 0.25, 0.125, 0.0625 };
	const double initial_input_gain = 0.5;
	//-------------------------------------------------

	// Init channel buffers
	for (int i = 0; i < MAX_NUM_CHANNEL; i++)
		for (int j = 0; j < BLOCK_SIZE; j++)
			sampleBuffer[i][j] = 0.0;

	// Open input and output wav files
	//-------------------------------------------------
	strcpy(WavInputName, argv[1]);
	wav_in = OpenWavFileForRead(WavInputName, "rb");
	strcpy(WavOutputName, argv[2]);
	wav_out = OpenWavFileForRead(WavOutputName, "wb");
	//-------------------------------------------------

	//Get output mode
	if (argc >= 4)
	{
		mode = atoi(argv[3]);
	}

	// Read input wav header
	//-------------------------------------------------
	ReadWavHeader(wav_in, inputWAVhdr);
	//-------------------------------------------------

	// Set up output WAV header
	//-------------------------------------------------	
	outputWAVhdr = inputWAVhdr;
	outputWAVhdr.fmt.NumChannels = get_out_num_ch(mode); // change number of channels according to processing

	int oneChannelSubChunk2Size = inputWAVhdr.data.SubChunk2Size / inputWAVhdr.fmt.NumChannels;
	int oneChannelByteRate = inputWAVhdr.fmt.ByteRate / inputWAVhdr.fmt.NumChannels;
	int oneChannelBlockAlign = inputWAVhdr.fmt.BlockAlign / inputWAVhdr.fmt.NumChannels;

	outputWAVhdr.data.SubChunk2Size = oneChannelSubChunk2Size * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.ByteRate = oneChannelByteRate * outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.BlockAlign = oneChannelBlockAlign * outputWAVhdr.fmt.NumChannels;


	// Write output WAV header to file
	//-------------------------------------------------
	WriteWavHeader(wav_out, outputWAVhdr);


	// Initialize echo 
	multitap_echo_init(&mEcho[0].echoState, mEcho[0].echoBuffer, ECHO_MAX_LENGTH, initial_delay, initial_input_gain, initial_gain, N_TAP);
	multitap_echo_init(&mEcho[1].echoState, mEcho[1].echoBuffer, ECHO_MAX_LENGTH, initial_delay, initial_input_gain, initial_gain, N_TAP);

	// Processing loop
	//-------------------------------------------------	
	{
		int sample;
		int BytesPerSample = inputWAVhdr.fmt.BitsPerSample / 8;
		const double SAMPLE_SCALE = -(double)(1 << 31);		//2^31
		unsigned int iNumSamples = inputWAVhdr.data.SubChunk2Size / (inputWAVhdr.fmt.NumChannels * inputWAVhdr.fmt.BitsPerSample / 8);

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

			processing(sampleBuffer, sampleBuffer, pmEcho, mode);

			for (int j = 0; j < BLOCK_SIZE; j++)
			{
				for (int k = 0; k < outputWAVhdr.fmt.NumChannels; k++)
				{
					if (is_channel_active(mode, k))
					{
						sample = (int)(sampleBuffer[k][j] * SAMPLE_SCALE);	// crude, non-rounding
						sample = sample >> (32 - inputWAVhdr.fmt.BitsPerSample);
						fwrite(&sample, outputWAVhdr.fmt.BitsPerSample / 8, 1, wav_out);
					}
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
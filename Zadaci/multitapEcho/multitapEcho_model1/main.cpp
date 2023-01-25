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
#include <stdlib.h>
#include <string.h>
#include "WAVheader.h"
#include "common.h"

/////////////////////////////////////////////////////////////////////////////////
// IO buffers
/////////////////////////////////////////////////////////////////////////////////
double sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Control state structure
/////////////////////////////////////////////////////////////////////////////////
typedef struct  
{
	double* pEchoBuff;
	int bufferLength;
	double* writePointer;
	double* readPointer[N_TAP];
	int delay[N_TAP];
	double input_gain;
	double tap_gain[N_TAP];
	int n_tap;
} EchoState;
/////////////////////////////////////////////////////////////////////////////////

// Multitap delay state and initialization constants
//-------------------------------------------------
EchoState echoState;
double echo_buffer[ECHO_MAX_LENGTH];
const int initial_delay[N_TAP] = {1024, 1536, 2560, 3072};
const double initial_gain[N_TAP] = {0.25, 0.125, 0.0625, 0.0625};
const double initial_input_gain = 0.5;
//-------------------------------------------------

/////////////////////////////////////////////////////////////////////////////////
// @Author	<student name>
// @Date		<date>  
//
// Function:
// multitap_echo_init
//
// @param - nothing
//
// @return - nothing
// Comment: Initialize echoState structure
//
// E-mail:	<email>
//
/////////////////////////////////////////////////////////////////////////////////
void multitap_echo_init()
{

	int i;
	double* pInitEchoBuffer = echo_buffer;

	for (i = 0; i < ECHO_MAX_LENGTH; i++)
	{
		*pInitEchoBuffer = 0.0;
		pInitEchoBuffer++;
	}

	echoState.pEchoBuff = echo_buffer;
	echoState.bufferLength = ECHO_MAX_LENGTH;
	echoState.writePointer = echoState.pEchoBuff + ECHO_MAX_LENGTH-1;
	echoState.input_gain = 0.5;
	echoState.n_tap = N_TAP;
	
	int* pDelay = echoState.delay;
	const int* pInitDelay = initial_delay;
	double* pGain = echoState.tap_gain;
	const double* pInitGain = initial_gain;
	double** readPtrPtr = echoState.readPointer;
	double* readPtrOffset = echoState.pEchoBuff + ECHO_MAX_LENGTH - 1;


	for(i = 0; i< N_TAP; i++)
	{		
		*readPtrPtr++ = readPtrOffset - *pInitDelay;
		*pDelay++ = *pInitDelay++;
		*pGain++ = *pInitGain++;
	}	
}


/////////////////////////////////////////////////////////////////////////////////
// @Author	<student name>
// @Date		<date>  
//
// Function:
// multitap_echo
//
// @param - pInbuf - Buffer with input samples
//		  - pOutbuf - Buffer with output samples
//
// @return - nothing
// Comment: Apply echo to input samples
//
// E-mail:	<email>
//
/////////////////////////////////////////////////////////////////////////////////
void multitap_echo(double *pInbuf, double *pOutbuf)
{
	int i, j;
	double* writePtr = echoState.writePointer;
	double* pBufEnd = echoState.pEchoBuff+echoState.bufferLength - 1;

	for(i = 0; i < BLOCK_SIZE; i++)
	{
		*writePtr = *pInbuf; 
		writePtr++;
		writePtr = (writePtr > pBufEnd)? echoState.pEchoBuff:writePtr;	

		double outputSample =  *pInbuf * echoState.input_gain;
		double* pGain = echoState.tap_gain;
		double** readPtrPtr = echoState.readPointer;
		
		for(j = 0; j < echoState.n_tap; j++)
		{
			double* readPtr = *readPtrPtr;
			outputSample +=  *pGain * *readPtr;
			pGain++;
			readPtr++;
			readPtr = (readPtr > pBufEnd)? echoState.pEchoBuff:readPtr;	
			*readPtrPtr = readPtr;
			readPtrPtr++;
		}

		*pOutbuf = outputSample;
		pOutbuf++;
		pInbuf++;
	}

	 echoState.writePointer = writePtr;
	 
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
	FILE *wav_in=NULL;
	FILE *wav_out=NULL;
	char WavInputName[256];
	char WavOutputName[256];
	WAV_HEADER inputWAVhdr,outputWAVhdr;	
	
	// Init channel buffers
	for(int i=0; i<MAX_NUM_CHANNEL; i++)
		memset(&sampleBuffer[i],0,BLOCK_SIZE);

	// Open input and output wav files
	//-------------------------------------------------
	strcpy(WavInputName,argv[1]);
	wav_in = OpenWavFileForRead (WavInputName,"rb");
	strcpy(WavOutputName,argv[2]);
	wav_out = OpenWavFileForRead (WavOutputName,"wb");
	//-------------------------------------------------

	// Read input wav header
	//-------------------------------------------------
	ReadWavHeader(wav_in,inputWAVhdr);
	//-------------------------------------------------
	
	// Set up output WAV header
	//-------------------------------------------------	
	outputWAVhdr = inputWAVhdr;
	outputWAVhdr.fmt.NumChannels = inputWAVhdr.fmt.NumChannels; // change number of channels

	int oneChannelSubChunk2Size = inputWAVhdr.data.SubChunk2Size/inputWAVhdr.fmt.NumChannels;
	int oneChannelByteRate = inputWAVhdr.fmt.ByteRate/inputWAVhdr.fmt.NumChannels;
	int oneChannelBlockAlign = inputWAVhdr.fmt.BlockAlign/inputWAVhdr.fmt.NumChannels;
	
	outputWAVhdr.data.SubChunk2Size = oneChannelSubChunk2Size*outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.ByteRate = oneChannelByteRate*outputWAVhdr.fmt.NumChannels;
	outputWAVhdr.fmt.BlockAlign = oneChannelBlockAlign*outputWAVhdr.fmt.NumChannels;


	// Write output WAV header to file
	//-------------------------------------------------
	WriteWavHeader(wav_out,outputWAVhdr);

	
	// Initialize echo 
	multitap_echo_init();
    

	// Processing loop
	//-------------------------------------------------	
	{
		int sample;
		int BytesPerSample = inputWAVhdr.fmt.BitsPerSample/8;
		const double SAMPLE_SCALE = -(double)(1 << 31);		//2^31
		int iNumSamples = inputWAVhdr.data.SubChunk2Size/(inputWAVhdr.fmt.NumChannels*inputWAVhdr.fmt.BitsPerSample/8);
		
		// exact file length should be handled correctly...
		for(int i=0; i<iNumSamples/BLOCK_SIZE; i++)
		{	
			for(int j=0; j<BLOCK_SIZE; j++)
			{
				for(int k=0; k<inputWAVhdr.fmt.NumChannels; k++)
				{	
					sample = 0; //debug
					fread(&sample, BytesPerSample, 1, wav_in);
					sample = sample << (32 - inputWAVhdr.fmt.BitsPerSample); // force signextend
					sampleBuffer[k][j] = sample / SAMPLE_SCALE;				// scale sample to 1.0/-1.0 range		
				}
			}
			
			
			// Call processing on first channel
			multitap_echo(sampleBuffer[0], sampleBuffer[0]);
			
			
			for(int j=0; j<BLOCK_SIZE; j++)
			{
				for(int k=0; k<outputWAVhdr.fmt.NumChannels; k++)
				{	
					sample = (int)(sampleBuffer[k][j] * SAMPLE_SCALE);	// crude, non-rounding 	
					sample = sample >> (32 - inputWAVhdr.fmt.BitsPerSample);
					fwrite(&sample, outputWAVhdr.fmt.BitsPerSample/8, 1, wav_out);		
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
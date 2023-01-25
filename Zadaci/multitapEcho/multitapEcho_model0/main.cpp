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

/////////////////////////////////////////////////////////////////////////////////
// Constant definitions
/////////////////////////////////////////////////////////////////////////////////
#define BLOCK_SIZE 16
#define MAX_NUM_CHANNEL 8
#define N_TAP 4
#define ECHO_MAX_LENGTH 4000
/////////////////////////////////////////////////////////////////////////////////

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
	int writeIndex;
	int readIndex[N_TAP];
	int delay[N_TAP];
	double input_gain;
	double tap_gain[N_TAP];
	int n_tap;
} EchoState;
/////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////////
// @Author	<student name>
// @Date		<date>  
//
// Function:
// multitap_echo_init
//
// @param - echoState - Control state structure
//		  - buffer - buffer for keeping delayed samples
//		  - echoBufLen - Length of buffer
//		  - delay - array containing delay values in number of samples
//		  - input_gain - gain to be applayed to input sample
//		  - tap_gain - array of gains to be applayed to each delayed sample
//		  - n_tap - number of taps (equals length of delay and tap_gain)
//
// @return - nothing
// Comment: Initialize echoState structure
//
// E-mail:	<email>
//
/////////////////////////////////////////////////////////////////////////////////
void multitap_echo_init(EchoState* echoState, double* buffer, const int echoBufLen, const int delay[], const double input_gain, const  double tap_gain[], const int n_tap)
{
	int i;
	for (i = 0; i < echoBufLen; i++)
	{
		buffer[i] = 0.0;
	}
	echoState->pEchoBuff = buffer;
	echoState->bufferLength = echoBufLen;
	echoState->writeIndex = echoBufLen-1;
	echoState->input_gain = input_gain;
	echoState->n_tap = n_tap;
	for(i = 0; i< n_tap; i++)
	{
		echoState->delay[i] = delay[i];
		echoState->readIndex[i] = echoBufLen - 1 - delay[i];
		echoState->tap_gain[i] = tap_gain[i];
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
//		  - inputLen - Length of input and output buffer
//		  - echoState - Control state structure
//
// @return - nothing
// Comment: Apply echo to input samples
//
// E-mail:	<email>
//
/////////////////////////////////////////////////////////////////////////////////
void multitap_echo(double *pInbuf, double *pOutbuf, int inputLen, EchoState* echoState)
{
	int i, j;

	for(i = 0; i < inputLen; i++)
	{
		echoState->pEchoBuff[echoState->writeIndex] = pInbuf[i];
		echoState->writeIndex = (echoState->writeIndex + 1) % echoState->bufferLength;	

		pOutbuf[i] =  pInbuf[i] * echoState->input_gain;
		for(j = 0; j < echoState->n_tap; j++)
		{
			pOutbuf[i] +=  echoState->tap_gain[j] * echoState->pEchoBuff[echoState->readIndex[j]];
			echoState->readIndex[j] = (echoState->readIndex[j] + 1) % echoState->bufferLength;
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
	FILE *wav_in=NULL;
	FILE *wav_out=NULL;
	char WavInputName[256];
	char WavOutputName[256];
	WAV_HEADER inputWAVhdr,outputWAVhdr;	


	// Multitap delay state and initialization constants
	//-------------------------------------------------
	EchoState echoState;
	double echo_buffer[ECHO_MAX_LENGTH];
	const int initial_delay[N_TAP] = {1024, 1536, 2560, 3072};
	const double initial_gain[N_TAP] ={0.25, 0.125, 0.0625, 0.0625};
	const double initial_input_gain = 0.5;
	//-------------------------------------------------

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
	multitap_echo_init(&echoState, echo_buffer, ECHO_MAX_LENGTH, initial_delay, initial_input_gain, initial_gain, N_TAP);
    

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
			multitap_echo(sampleBuffer[0], sampleBuffer[0], BLOCK_SIZE, &echoState);
			
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
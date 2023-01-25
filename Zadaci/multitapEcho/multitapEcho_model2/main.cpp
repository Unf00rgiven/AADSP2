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
#include "stdfix_emu.h"
#include "fixed_point_math.h"
#include "common.h"

/////////////////////////////////////////////////////////////////////////////////
// IO buffers
/////////////////////////////////////////////////////////////////////////////////
DSPfract sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];
/////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////
// Control state structure
/////////////////////////////////////////////////////////////////////////////////

typedef struct  
{
	DSPfract* pEchoBuff;
	DSPint bufferLength;
	DSPfract* writePointer;
	DSPfract* readPointer[N_TAP];
	DSPint delay[N_TAP];
	DSPfract input_gain;
	DSPfract tap_gain[N_TAP];
	DSPint n_tap;
} EchoState;


// Multitap delay state and initialization constants
//-------------------------------------------------
EchoState echoState;
DSPfract echo_buffer[ECHO_MAX_LENGTH];
const DSPint initial_delay[N_TAP] = {1024, 1536, 2560, 3072};
const DSPfract initial_gain[N_TAP] = {FRACT_NUM(0.25), FRACT_NUM(0.125), FRACT_NUM(0.0625), FRACT_NUM(0.0625)};
const DSPfract initial_input_gain = FRACT_NUM(0.5);
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
	DSPint i;
	DSPfract* pInitEchoBuffer = echo_buffer;


	for (i = 0; i < ECHO_MAX_LENGTH; i++)
	{
		*pInitEchoBuffer = FRACT_NUM(0.0);
		pInitEchoBuffer++;
	}

	echoState.pEchoBuff = echo_buffer;
	echoState.bufferLength = ECHO_MAX_LENGTH;
	echoState.writePointer = echoState.pEchoBuff + ECHO_MAX_LENGTH-1;
	echoState.input_gain = FRACT_NUM(0.5);
	echoState.n_tap = N_TAP;
	
	DSPint* pDelay = echoState.delay;
	const DSPint* pInitDelay = initial_delay;
	DSPfract* pGain = echoState.tap_gain;
	const DSPfract* pInitGain = initial_gain;
	DSPfract** readPtrPtr = echoState.readPointer;
	DSPfract* readPtrOffset = echoState.pEchoBuff + ECHO_MAX_LENGTH - 1;


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
void multitap_echo(DSPfract *pInbuf, DSPfract *pOutbuf)
{
	DSPint i, j;
	DSPfract* writePtr = echoState.writePointer;
	DSPfract* pBufEnd = echoState.pEchoBuff+echoState.bufferLength - 1;

	for(i = 0; i < BLOCK_SIZE; i++)
	{
		*writePtr = *pInbuf; 
		writePtr++;
		writePtr = (writePtr > pBufEnd)? echoState.pEchoBuff:writePtr;	

		DSPfract outputSample =  *pInbuf * echoState.input_gain;
		DSPfract* pGain = echoState.tap_gain;
		DSPfract** readPtrPtr = echoState.readPointer;
		
		for(j = 0; j < echoState.n_tap; j++)
		{
			DSPfract* readPtr = *readPtrPtr;
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
DSPint main(DSPint argc, char* argv[])
{
	FILE *wav_in=NULL;
	FILE *wav_out=NULL;
	char WavInputName[256];
	char WavOutputName[256];
	WAV_HEADER inputWAVhdr,outputWAVhdr;	

	// Init channel buffers
	for(DSPint i=0; i<MAX_NUM_CHANNEL; i++)
		for(DSPint j=0; j<BLOCK_SIZE; j++)
			sampleBuffer[i][j] = FRACT_NUM(0.0);


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

	DSPint oneChannelSubChunk2Size = inputWAVhdr.data.SubChunk2Size/inputWAVhdr.fmt.NumChannels;
	DSPint oneChannelByteRate = inputWAVhdr.fmt.ByteRate/inputWAVhdr.fmt.NumChannels;
	DSPint oneChannelBlockAlign = inputWAVhdr.fmt.BlockAlign/inputWAVhdr.fmt.NumChannels;
	
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
		DSPint sample;
		DSPint BytesPerSample = inputWAVhdr.fmt.BitsPerSample/8;
		const double SAMPLE_SCALE = -(double)(1 << 31);		//2^31
		DSPint iNumSamples = inputWAVhdr.data.SubChunk2Size/(inputWAVhdr.fmt.NumChannels*inputWAVhdr.fmt.BitsPerSample/8);
		
		// exact file length should be handled correctly...
		for(int i=0; i<iNumSamples/BLOCK_SIZE; i++)
		{	
			for(DSPint j=0; j<BLOCK_SIZE; j++)
			{
				for(DSPint k=0; k<inputWAVhdr.fmt.NumChannels; k++)
				{	
					sample = 0; //debug
					fread(&sample, BytesPerSample, 1, wav_in);
					sample = sample << (32 - inputWAVhdr.fmt.BitsPerSample); // force signextend
					sampleBuffer[k][j] = sample / SAMPLE_SCALE;				// scale sample to 1.0/-1.0 range		
				}
			}
			
						
			// Call processing on first channel
			multitap_echo(sampleBuffer[0], sampleBuffer[0]);
			
						
			for(DSPint j=0; j<BLOCK_SIZE; j++)
			{
				for(DSPint k=0; k<outputWAVhdr.fmt.NumChannels; k++)
				{	
					sample = sampleBuffer[k][j].toLong();	// crude, non-rounding 			
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
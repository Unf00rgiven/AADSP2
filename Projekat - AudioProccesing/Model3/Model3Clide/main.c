#include <dsplib\wavefile.h>
#include <stdio.h>
#include <stdfix.h>
#include <string.h>
#include <stdlib.h>
#include "common.h"

// IO Buffers
__memY DSPfract sampleBuffer[MAX_NUM_CHANNEL][BLOCK_SIZE];

// Mode
DSPint Mode;
// Filter enable
DSPint LPFenable;

// Default variable gains
DSPfract variablesGain[DEFAULT_OUTPUT_NUM_CHANNELS][INPUT_NUM_CHANNELS] = {{MINUS_4DB, PLUS_4DB }, { MINUS_4DB, PLUS_4DB }, { MINUS_4DB, PLUS_4DB }, { MINUS_4DB, PLUS_4DB }, { MINUS_4DB, PLUS_4DB }};

// Intermediate gained values
DSPaccum gained[INPUT_NUM_CHANNELS];
DSPfract out[DEFAULT_OUTPUT_NUM_CHANNELS];

// Filter buffers
DSPfract history[DEFAULT_OUTPUT_NUM_CHANNELS][FILTERLEN];

DSPfract* pStateL = *(history + LEFT_CH);
DSPfract* pStateR = *(history + RIGHT_CH);
DSPfract* pStateLS = *(history + LS_CH);
DSPfract* pStateRS = *(history + RS_CH);
DSPfract* pStateC = *(history + C_CH);

DSPfract filterCoeff[FILTERLEN] = {
	FRACT_NUM(-0.00032348877629069159),
	FRACT_NUM(0.00061391313136107599),
	FRACT_NUM(-0.00098367441013966672),
	FRACT_NUM(0.00135993640821725180),
	FRACT_NUM(-0.00157105555507173080),
	FRACT_NUM(0.00129463985232006930),
	FRACT_NUM(-0.00000265479966322842),
	FRACT_NUM(-0.00307975028120704550),
	FRACT_NUM(0.00897229562606529850),
	FRACT_NUM(-0.01884974934416482400),
	FRACT_NUM(0.03380441697827836500),
	FRACT_NUM(-0.05439224107032499700),
	FRACT_NUM(0.07990079675574889000),
	FRACT_NUM(-0.10731420700376081000),
	FRACT_NUM(0.13003365984943829000),
	FRACT_NUM(0.86107432527838756000),
	FRACT_NUM(0.13003365984943829000),
	FRACT_NUM(-0.10731420700376081000),
	FRACT_NUM(0.07990079675574889000),
	FRACT_NUM(-0.05439224107032499700),
	FRACT_NUM(0.03380441697827836500),
	FRACT_NUM(-0.01884974934416482400),
	FRACT_NUM(0.00897229562606529850),
	FRACT_NUM(-0.00307975028120704550),
	FRACT_NUM(-0.00000265479966322842),
	FRACT_NUM(0.00129463985232006930),
	FRACT_NUM(-0.00157105555507173080),
	FRACT_NUM(0.00135993640821725180),
	FRACT_NUM(-0.00098367441013966672),
	FRACT_NUM(0.00061391313136107599),
	FRACT_NUM(-0.00032348877629069159)
};

void FilterInit()
{
	int j;
	DSPfract* historyPtr1 = *(history + LEFT_CH);
	DSPfract* historyPtr2 = *(history + RIGHT_CH);
	DSPfract* historyPtr3 = *(history + LS_CH);
	DSPfract* historyPtr4 = *(history + RS_CH);
	DSPfract* historyPtr5 = *(history + C_CH);

	for (j = 0; j < FILTERLEN; j++)
	{
		*historyPtr1 = FRACT_NUM(0.0);
		*historyPtr2 = FRACT_NUM(0.0);
		*historyPtr3 = FRACT_NUM(0.0);
		*historyPtr4 = FRACT_NUM(0.0);
		*historyPtr5 = FRACT_NUM(0.0);

		historyPtr1++;
		historyPtr2++;
		historyPtr3++;
		historyPtr4++;
		historyPtr5++;
	}
	*pStateL = FRACT_NUM(0.0);
	*pStateR = FRACT_NUM(0.0);
	*pStateLS = FRACT_NUM(0.0);
	*pStateRS = FRACT_NUM(0.0);
	*pStateC = FRACT_NUM(0.0);
}

DSPfract saturation(DSPfract input)
{
	DSPaccum inValue = input;

	if (input > (DSPaccum)THRESHOLD)
	{
		return THRESHOLD;
	}
	if (input < (DSPaccum)-THRESHOLD)
	{
		return -THRESHOLD;
	}

	return input;
}


DSPfract fir_circular(DSPfract input, DSPfract **p_state, DSPfract *state_start, DSPfract *state_end)
{
	DSPint i;
	DSPfract* state = *p_state;
	DSPaccum ret_val;

	DSPfract* ptrCoeffs = (filterCoeff + FILTERLEN - 1);
	/* copy the filter's state to a local */

	/* store input at the beginning of the delay line */
	*state = input;					//history[state] = input;
	if (++state >= state_end)
	{								/* incr state and check for wrap */
		state = state_start;
	}

	/* calc FIR and shift data */
	ret_val = FRACT_NUM(0.0);
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

void Processing(__memY DSPfract pIn[][BLOCK_SIZE], DSPfract __memY pOut[][BLOCK_SIZE])
{
	DSPint i;

		// In samples
		__memY DSPfract* ptrInL = *(pIn + LEFT_CH);
		__memY DSPfract* ptrInR = *(pIn + RIGHT_CH);

		// Out pointers
		__memY DSPfract* ptrOutL = *(pOut + LEFT_CH);
		__memY DSPfract* ptrOutR = *(pOut + RIGHT_CH);
		__memY DSPfract* ptrOutLS = *(pOut + LS_CH);
		__memY DSPfract* ptrOutRS = *(pOut + RS_CH);
		__memY DSPfract* ptrOutC = *(pOut + C_CH);

		for (i = 0; i < BLOCK_SIZE; i++)
		{
			// Left Channel processing
			*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + LEFT_CH))));				// L input * L gain
			*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + LEFT_CH))));				// R input * -L scaled gain
			*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
			*(out + LEFT_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);							// L channel = gained L + gained R

			// Right Channel processing
			*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + RIGHT_CH))));				// L input * R gain
			*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + RIGHT_CH))));				// R input * -R scaled gain
			*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
			*(out + RIGHT_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);							// R channel = gained L + gained R

			// Ls Channel processing
			*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + LS_CH))));					// L input * Ls gain
			*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + LS_CH))));					// R input * -Ls scaled gain
			*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
			*(out + LS_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);							// Ls channel = gained L + gained R

			// Rs Channel processing
			*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + RS_CH))));					// L input * Rs gain
			*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + RS_CH))));					// R input * -Rs scaled gain
			*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
			*(out + RS_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);							// Rs channel = gained L + gained R

			// Center Channel processing
			*(gained + LEFT_CH) = (*ptrInL) * (*(0 + (*(variablesGain + C_CH))));					// L input * C gain
			*(gained + RIGHT_CH) = (*ptrInR) * (*(1 + (*(variablesGain + C_CH))));					// R input * -C scaled gain
			*(gained + RIGHT_CH) = *(gained + RIGHT_CH) << 4;										// back to scope
			*(out + C_CH) = *(gained + LEFT_CH) + *(gained + RIGHT_CH);								// C channel = gained L + gained R

			// Direct output without filtering, SRS saturation after +
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

			// Saturiation before writing in output
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


	// Filter enable
    LPFenable = 1;
    // Mode set
	Mode = 1;

	// Filter init
	FilterInit();

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

	// Set up output WAV header
	//-------------------------------------------------
	strcpy(WavOutputName,argv[1]);
	// Channels mode
	if(Mode == 0)
	{
		outChannels = OUTPUT_NUM_CHANNELS_2;
		wav_out = cl_wavwrite_open(WavOutputName, bitsPerSample, outChannels, sampleRate);
			if(!wav_out)
		    {
		        printf("Error: Could not open wavefile.\n");
		        return -1;
		    }
	}
	else {
		outChannels = DEFAULT_OUTPUT_NUM_CHANNELS ;
		wav_out = cl_wavwrite_open(WavOutputName, bitsPerSample, outChannels, sampleRate);
			if(!wav_out)
		    {
		        printf("Error: Could not open wavefile.\n");
		        return -1;
		    }
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


				////////////////////////////////////////////////////////////
				Processing(sampleBuffer, sampleBuffer);
				////////////////////////////////////////////////////////////
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

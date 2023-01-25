
#include "multitap_echo.h"

const DSPint initial_delay[N_TAP] = { 1024, 1536, 2560, 3072 };
const DSPfract initial_gain[N_TAP] = { FRACT_NUM(0.75), FRACT_NUM(0.25), FRACT_NUM(0.125), FRACT_NUM(0.0625) };
const DSPfract initial_input_gain = FRACT_NUM(0.5);


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
void multitap_echo_init(MultitapEcho* multitapEchoState)
{
	DSPfract* pInitEchoBuffer = multitapEchoState->echoBuffer;
	EchoState* pInitEchoState = &multitapEchoState->echoState;

	EchoState echoState = *pInitEchoState;

	DSPint i;

	pInitEchoState->pEchoBuff = pInitEchoBuffer;
	pInitEchoState->bufferLength = ECHO_MAX_LENGTH;
	pInitEchoState->writePointer = pInitEchoState->pEchoBuff + ECHO_MAX_LENGTH - 1;
	pInitEchoState->input_gain = initial_input_gain;
	pInitEchoState->n_tap = N_TAP;

	for (i = 0; i < ECHO_MAX_LENGTH; i++)
	{
		*pInitEchoBuffer = FRACT_NUM(0.0);
		pInitEchoBuffer++;
	}



	DSPint* pDelay = pInitEchoState->delay;
	const DSPint* pInitDelay = initial_delay;
	DSPfract* pGain = pInitEchoState->tap_gain;
	const DSPfract* pInitGain = initial_gain;
	DSPfract** readPtrPtr = pInitEchoState->readPointer;
	DSPfract* readPtrOffset = pInitEchoState->pEchoBuff + ECHO_MAX_LENGTH - 1;


	for (i = 0; i< N_TAP; i++)
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
//		  - echoState - Control state structure
//
// @return - nothing
// Comment: Apply echo to input samples
//
// E-mail:	<email>
//
/////////////////////////////////////////////////////////////////////////////////
void multitap_echo(DSPfract* pInbuf, DSPfract* pOutbuf, EchoState* echoStatePtr)
{
	DSPint i, j;
	DSPfract* writePtr = echoStatePtr->writePointer;
	DSPfract* pBufEnd = echoStatePtr->pEchoBuff + echoStatePtr->bufferLength - 1;
	for (i = 0; i < BLOCK_SIZE; i++)
	{
		*writePtr = *pInbuf;
		writePtr++;
		writePtr = (writePtr > pBufEnd) ? echoStatePtr->pEchoBuff : writePtr;

		//DSPfract outputSample = *pInbuf * echoStatePtr->input_gain;
		DSPaccum a = FRACT_NUM(0.0);
		a = *pInbuf * echoStatePtr->input_gain;
		a = a >> 1;
		DSPaccum outputSample = a;
		DSPfract* pGain = echoStatePtr->tap_gain;
		DSPfract** readPtrPtr = echoStatePtr->readPointer;

		for (j = 0; j < echoStatePtr->n_tap; j++)
		{
			DSPfract* readPtr = *readPtrPtr;
			a = *pGain * *readPtr;
			a = a >> 1;
			//outputSample += *pGain * *readPtr;
			outputSample += a;
			pGain++;
			readPtr++;
			readPtr = (readPtr > pBufEnd) ? echoStatePtr->pEchoBuff : readPtr;
			*readPtrPtr = readPtr;
			readPtrPtr++;
		}
		outputSample = outputSample << 1;
		*pOutbuf = outputSample;
		pOutbuf++;
		pInbuf++;
	}

	echoStatePtr->writePointer = writePtr;

}


#include "multitap_echo.h"

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
void multitap_echo_init(EchoState* echoState, double buffer[], const int echoBufLen, const int delay[], const double inputGain, const  double tapGain[], const int nTap)
{
	int i;
	for (i = 0; i < echoBufLen; i++)
	{
		buffer[i] = 0.0;
	}
	echoState->pEchoBuff = buffer;
	echoState->bufferLength = echoBufLen;
	echoState->writeIndex = echoBufLen - 1;
	echoState->inputGain = inputGain;
	echoState->nTap = nTap;
	for (i = 0; i < nTap; i++)
	{
		echoState->delay[i] = delay[i];
		echoState->readIndex[i] = echoBufLen - 1 - delay[i];
		echoState->tapGain[i] = tapGain[i];
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
void multitap_echo(double* pInbuf, double* pOutbuf, int inputLen, EchoState* echoState)
{
	int i, j;

	for (i = 0; i < inputLen; i++)
	{
		echoState->pEchoBuff[echoState->writeIndex] = pInbuf[i];
		echoState->writeIndex = (echoState->writeIndex + 1) % echoState->bufferLength;

		pOutbuf[i] = pInbuf[i] * echoState->inputGain;
		for (j = 0; j < echoState->nTap; j++)
		{
			pOutbuf[i] += echoState->tapGain[j] * echoState->pEchoBuff[echoState->readIndex[j]];
			echoState->readIndex[j] = (echoState->readIndex[j] + 1) % echoState->bufferLength;
		}
	}
}

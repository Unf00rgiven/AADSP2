#ifndef MULTITAP_ECHO_H
#define MULTITAP_ECHO_H

#include "common.h"

#if __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////////////
// Constant definitions
/////////////////////////////////////////////////////////////////////////////////
#define N_TAP 4
#define ECHO_MAX_LENGTH 4000
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
	double inputGain;
	double tapGain[N_TAP];
	int nTap;
} EchoState;

typedef struct
{
	EchoState echoState;
	double echoBuffer[ECHO_MAX_LENGTH];
} MultitapEcho;
/////////////////////////////////////////////////////////////////////////////////


void multitap_echo_init(EchoState* echoState, double buffer[], const int echoBufLen, const int delay[], const double input_gain, const  double tap_gain[], const int n_tap);

void multitap_echo(double* pInbuf, double* pOutbuf, int inputLen, EchoState* echoState);


#if __cplusplus
}
#endif

#endif

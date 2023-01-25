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
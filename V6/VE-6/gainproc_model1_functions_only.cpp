void initGainProcessing(double preGainValue, double* defaultVariablesGain, double postGainValue)
{
	preGain = preGainValue;
	for (int i = 0; i < INPUT_NUM_CHANNELS; i++)
	{
		variablesGain[i] = defaultVariablesGain[i];
	}
	postGain = postGainValue;
}

double saturation(double in)
{
	// Simple limiter since we know that pre-Gain adds 6dB
	if (in > limiterThreshold)
	{
		return fmin(in, limiterThreshold);
	}
	else if (in < -limiterThreshold)
	{
		return fmax(in, -limiterThreshold);
	}

	return in;
}

void gainProcessing(double pIn[][BLOCK_SIZE], double pOut[][BLOCK_SIZE])
{

	// get address of the first sample in the current channel
	double* samplePtrInL = *(pIn + LEFT_CH);
	double* samplePtrInR = *(pIn + RIGHT_CH);
	double* samplePtrOutL = *(pOut + LEFT_CH);
	double* samplePtrOutR = *(pOut + RIGHT_CH);
	double* centerSamplePtr = *(pOut + CENTER_CH);		
	double* variablesGainPtrL = variablesGain + LEFT_CH;
	double* variablesGainPtrR = variablesGain + RIGHT_CH;

	for (int i = 0; i < BLOCK_SIZE; i++)
	{
		// first stage, apply constant pre-Gain
		*samplePtrInL = *samplePtrInL * preGain;
		*samplePtrInR = *samplePtrInR * preGain;

		// second stage, apply variable gain
		*samplePtrOutL = saturation(*samplePtrInL++ * *variablesGainPtrL);
		*samplePtrOutR = saturation(*samplePtrInR++ * *variablesGainPtrR);

		// add processed samples to the center output channel
		*centerSamplePtr = *samplePtrOutL++ + *samplePtrOutR++;
		// apply center channel post-Gain
		*centerSamplePtr++ *= postGain;
	}
}
/*
 *
 * This element can act as a dynamic range compressor. A compressor changes the
 * amplitude of all samples above a specific threshold with a specific ratio. If
 * soft-knee mode is selected the ratio is applied smoothly.
 *
 * parameters: ratio [0.0, 1.0)
 *             threshold [0.0, 1.0)
 *             type {CHARACTERISTICS_HARD_KNEE, CHARACTERISTICS_SOFT_KNEE}
 */



typedef enum __characteristic
{
	CHARACTERISTICS_HARD_KNEE = 0,
	CHARACTERISTICS_SOFT_KNEE
} characteristic_t;


typedef struct __AudioCompressor
{
	int characteristics;
	float threshold;
	float ratio;
} AudioCompressor_t;



static void
audio_compressor_init(AudioCompressor_t * compressor)
{
	compressor->ratio = 1.0;
	compressor->threshold = 0.0;
	compressor->characteristics = CHARACTERISTICS_HARD_KNEE;
}

static void
gst_audio_dynamic_transform_compressor_float(AudioCompressor_t *
compressor, float * data, unsigned int num_samples)
{
	double val, threshold = compressor->threshold;
	int i;

	/* Nothing to do for us if ratio == 1.0. */
	if (compressor->ratio == 1.0)
		return;

	if (compressor->characteristics == CHARACTERISTICS_HARD_KNEE)
	{
		for (i = 0; i < num_samples; i++) {
			val = data[i];

			if (val > threshold) {
				val = threshold + (val - threshold) * compressor->ratio;
			}
			else if (val < -threshold) {
				val = -threshold + (val + threshold) * compressor->ratio;
			}
			data[i] = (float)val;
		}

	}
	else{
		double a_p, b_p, c_p;
		double a_n, b_n, c_n;

		/* We build a 2nd degree polynomial here for
		 * values greater than threshold or small than
		 * -threshold with:
		 * f(t) = t, f'(t) = 1, f'(m) = r
		 * =>
		 * a = (1-r)/(2*(t-m))
		 * b = (r*t - m)/(t-m)
		 * c = t * (1 - b - a*t)
		 * f(x) = ax^2 + bx + c
		 */

		/* If treshold is the same as the maximum
		 * we need to raise it a bit to prevent
		 * division by zero. */
		if (threshold == 1.0)
			threshold = 1.0 + 0.00001;

		a_p = (1.0 - compressor->ratio) / (2.0 * (threshold - 1.0));
		b_p = (compressor->ratio * threshold - 1.0) / (threshold - 1.0);
		c_p = threshold * (1.0 - b_p - a_p * threshold);
		a_n = (1.0 - compressor->ratio) / (2.0 * (-threshold + 1.0));
		b_n = (-compressor->ratio * threshold + 1.0) / (-threshold + 1.0);
		c_n = -threshold * (1.0 - b_n + a_n * threshold);

		for (i = 0; i < num_samples; i++) {
			val = data[i];

			if (val > 1.0) {
				val = 1.0 + (val - 1.0) * compressor->ratio;
			}
			else if (val > threshold) {
				val = a_p * val * val + b_p * val + c_p;
			}
			else if (val < -1.0) {
				val = -1.0 + (val + 1.0) * compressor->ratio;
			}
			else if (val < -threshold) {
				val = a_n * val * val + b_n * val + c_n;
			}
			data[i] = (float)val;
		}
	}
   
}
/*
 *
 * This element can act as a dynamic range expander. A expander changes the
 * amplitude of all samples below a specific threshold with a specific ratio
 * If soft-knee mode is selected the ratio is applied smoothly.
 *
 * parameters: ratio [0.0, 8.0)
 *             threshold [0.0, 1.0)
 *             type {CHARACTERISTICS_HARD_KNEE, CHARACTERISTICS_SOFT_KNEE}
 */



typedef enum __characteristic
{
	CHARACTERISTICS_HARD_KNEE = 0,
	CHARACTERISTICS_SOFT_KNEE
} characteristic_t;


typedef struct __AudioExpander
{
	int characteristics;
	float threshold;
	float ratio;
} AudioExpander_t;



static void
audio_expander_init(AudioExpander_t * expander)
{
	expander->ratio = 1.0;
	expander->threshold = 0.0;
	expander->characteristics = CHARACTERISTICS_HARD_KNEE;
}


static void
gst_audio_dynamic_transform_expander_float(AudioExpander_t * expander,
float * data, unsigned int num_samples)
{
	double val, threshold = expander->threshold, zero;
	double a_p, b_p, c_p;
	double a_n, b_n, c_n;
	double r2;
	int i;

	/* Nothing to do for us here if threshold equals 0.0
	 * or ratio equals 1.0 */
	if (expander->threshold == 0.0 || expander->ratio == 1.0)
		return;


	if (expander->characteristics == CHARACTERISTICS_HARD_KNEE)
	{
		/* zero crossing of our function */
		if (expander->ratio != 0.0)
			zero = threshold - threshold / expander->ratio;
		else
			zero = 0.0;

		if (zero < 0.0)
			zero = 0.0;

		for (i = 0; i < num_samples; i++) {
			val = data[i];

			if (val < threshold && val > zero) {
				val = expander->ratio * val + threshold * (1.0 - expander->ratio);
			}
			else if ((val <= zero && val > 0.0) || (val >= -zero && val < 0.0)) {
				val = 0.0;
			}
			else if (val > -threshold && val < -zero) {
				val = expander->ratio * val - threshold * (1.0 - expander->ratio);
			}
			data[i] = (float)val;
		}
	}
	else
	{
		/* zero crossing of our function */
		zero = (threshold * (expander->ratio - 1.0)) / (1.0 + expander->ratio);

		if (zero < 0.0)
			zero = 0.0;

		/* We build a 2n degree polynomial here for values between
		 * 0 and threshold or 0 and -threshold with:
		 * f(t) = t, f'(t) = 1, f(z) = 0, f'(z) = r
		 * z between 0 and t
		 * =>
		 * a = (1 - r^2) / (4 * t)
		 * b = (1 + r^2) / 2
		 * c = t * (1.0 - b - a*t)
		 * f(x) = ax^2 + bx + c */
		r2 = expander->ratio * expander->ratio;
		a_p = (1.0 - r2) / (4.0 * threshold);
		b_p = (1.0 + r2) / 2.0;
		c_p = threshold * (1.0 - b_p - a_p * threshold);
		a_n = (1.0 - r2) / (-4.0 * threshold);
		b_n = (1.0 + r2) / 2.0;
		c_n = -threshold * (1.0 - b_n + a_n * threshold);

		for (i = 0; i < num_samples; i++) {
			val = data[i];

			if (val < threshold && val > zero) {
				val = a_p * val * val + b_p * val + c_p;
			}
			else if ((val <= zero && val > 0.0) || (val >= -zero && val < 0.0)) {
				val = 0.0;
			}
			else if (val > -threshold && val < -zero) {
				val = a_n * val * val + b_n * val + c_n;
			}
			data[i] = (float)val;
		}
	}
}


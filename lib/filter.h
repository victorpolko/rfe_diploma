// Designed filter
void Filter (const double in[], double out[], int sizeIn) {
  const int N = 20; // Filter length

  double Fd = 2000; // Input data sample rate (frequency<data>)
  double Fs = 20; // Bandwidth frequency
  double Fx = 50; // Stopband frequency

  double H [N]    = {0}; // Filter impulse response
  double H_id [N] = {0}; // Ideal filter impulse response
  double W [N]    = {0}; // Weight function
  double SUM      = 0;

  // Filter impulse response calculation
  double Fc = (Fs + Fx) / (2 * Fd);

  int i, j = 0;
  for (i = 0; i < N; i++) {
    if (i == 0) {
      H_id[i] = 2 * PI * Fc;
    } else {
      H_id[i] = sin(2 * PI * Fc * i) / (PI * i);
    }

    // Blackman window function
    W [i] = 0.42 - 0.5 * cos((2*PI*i) /( N-1)) + 0.08 * cos((4*PI*i) /( N-1));
    H [i] = H_id[i] * W[i];
  }

  // Impulse response normalization
  for (i = 0; i < N; i++) SUM += H[i];
  for (i = 0; i < N; i++) H[i] /= SUM; // The sum of coefficients is equal to 1

  // Input data filtering
  for (i = 0; i < sizeIn; i++) {
    out[i] = 0.;

    for (j = 0; j < N - 1; j++) {
      out[i]+= H[j] * in[i - j]; // The filter formula
    }
  }
}

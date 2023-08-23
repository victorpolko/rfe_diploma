// LPF Coefficients
float lpf_b01 = -0.0036, lpf_b11 = 0.0093,
      lpf_a11 = -1.8901, lpf_a21 = 0.8960;

float lpf_rn, lpf_rn1,
      lpf_yn, lpf_y1n1, lpf_y1n2;

// BPF Coefficients
float bpf_coeff = 0.4557;
float bpf_b01 = -0.2461, bpf_b11 =  0.0480,
      bpf_a11 = -1.4601, bpf_a21 =  0.6700,
      bpf_b02 = -0.0733, bpf_b12 =  0.5676,
      bpf_a12 = -1.4974, bpf_a22 =  0.4466;

float bpf_rn,  bpf_rn1,  bpf_rn2,
      bpf_y1n, bpf_y1n1, bpf_y1n2,
      bpf_y2n, bpf_y2n1, bpf_y2n2,
      bpf_yn;

/**** Lowpass Filter ****/
// Prepare LPF Coefficients
// @return Updates first-step output variables
void prepare_lpf(void) {
  lpf_rn   = ADC_result(0);
  lpf_yn  = lpf_b01 * lpf_rn;

  lpf_y1n1 = lpf_yn;
  lpf_rn1  = lpf_rn;
  lpf_rn   = ADC_result(0);
  lpf_yn   = lpf_b01*lpf_rn + lpf_b11*lpf_rn1 - lpf_a11*lpf_y1n1;
}

// Apply LPF to ADC result
// @return Calculates output value
void breath_processing(void) {
  lpf_y1n2 = lpf_y1n1;
  lpf_y1n1 = lpf_yn;
  lpf_rn1  = lpf_rn;
  lpf_rn   = ADC_result(0);

  lpf_yn = lpf_b01*lpf_rn + lpf_b11*lpf_rn1 - lpf_a11*lpf_y1n1 - lpf_a21*lpf_y1n2;

  lpf_output(lpf_yn);

  // Update LCD output
  sprintf(string, "%.2f", lpf_yn);
  sprintf(string, "%.2f", lpf_rn);
}

/**** Bandpass Filter ****/
// Calculate links output
// @return Assigns output to corresponding variables
void get_outputs(void) {
  bpf_y1n = bpf_b01*bpf_rn + bpf_b11*bpf_rn1 - bpf_a11*bpf_y1n1 - bpf_a21*bpf_y1n2;
  bpf_y2n = bpf_b02*bpf_rn + bpf_b12*bpf_rn1 - bpf_a12*bpf_y2n1 - bpf_a22*bpf_y2n2;
}

// Prepare BPF Coefficients
// @return Writes first-step output variables
void prepare_bpf(void) {
  // Get ADC as input
  bpf_rn   = ADC_result(0);

  // Get output
  bpf_y1n  = bpf_b01 * bpf_rn;
  bpf_y2n  = bpf_b02 * bpf_rn;

  // Shift input one step back
  bpf_rn1  = bpf_rn;
  bpf_rn   = ADC_result(0);

  // Shift output one step back
  bpf_y1n1 = bpf_y1n;
  bpf_y2n1 = bpf_y2n;

  get_outputs();
}

// Apply BPF to ADC result
// @return Calculates output value
void heart_processing(void) {
  // Shift input one step back
  bpf_rn2  = bpf_rn1;
  bpf_rn1  = bpf_rn;
  bpf_rn   = ADC_result(0);

  // Shift output one step back
  bpf_y1n2 = bpf_y1n1;
  bpf_y1n1 = bpf_y1n;

  // Calculate output value
  get_outputs();
  bpf_yn = bpf_y1n + bpf_y2n + bpf_rn*bpf_coeff;

  bpf_output(bpf_yn);
}

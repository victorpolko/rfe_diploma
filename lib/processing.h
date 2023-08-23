// Average Breath rate variables
float lpf_window[3];
float lpf_total;
int   lpf_n = sizeof(lpf_window)/sizeof(lpf_window[0]) - 1;

// Average Heart rate variables
float bpf_window[5];
float bpf_total;
int   bpf_n = sizeof(bpf_window)/sizeof(bpf_window[0]) - 1;

unsigned int flag1, flag2;
char string[10];
int i,k;

// Average Breath rate calculation with window
// @param time [float] Counter 1 value
// @return [float] Breath rate per minute averaged for 3 inhales
float breath_average(float time) {
  // Reset accumulator
  lpf_total = 0;

  // Write current time to window array
  lpf_window[lpf_n] = 60/time;
  // Decrease counter so that it is a real window
  lpf_n--;

  // Reset counter when it comes to 0
  if (lpf_n < 0) {
    lpf_n = sizeof(lpf_window)/sizeof(lpf_window[0]) - 1;
  }

  // Accumulate array total
  for(i = 0, k = 0; i < 3; i++) {
    if (lpf_window[i] > 0) {
      lpf_total += lpf_window[i];
      k ++;
    }
  }
  // Return average
  return lpf_total/k;
}

// Average Heart rate calculation with window
// @param time [float] Counter 2 value
// @return [float] Breath rate per minute averaged for 5 heart beats
float heart_average(float time) {
  // Reset accumulator
  bpf_total = 0;

  // Write current time to window array
  bpf_window[bpf_n] = 60/time;
  // Decrease counter so that it is a real window
  bpf_n--;

  // Reset counter when it comes to 0
  if (bpf_n < 0) {
    bpf_n = sizeof(bpf_window)/sizeof(bpf_window[0]) - 1;
  }

  // Accumulate array total
  for(i = 0, k = 0; i < 5; i++) {
    if (bpf_window[i] > 0) {
      bpf_total += bpf_window[i];
      k ++;
    }
  }
  // Return average
  return bpf_total/k;
}

// Output Breath rate on LCD
// @param lpf_val [float] Filtered Breath signal value
// @return Sends Average Breath Rate to LCD
void lpf_output(float lpf_val) {
  // Wait 1.5 s for stability
  if (counter1_value > 1.5) {

    // Set treshold lpf_value value to 0.5
    if (lpf_val >= 512) {
      // Counter 1 Handling
      if  (flag1 == 0) {
        // Show calculated average breath rate on LCD
        sprintf(string, "%.2f", breath_average(counter1_value));
        lcd_gotoxy(10,1);
        lcd_puts(string);

        // Reset Counter 1
        TCNT1 = 0;
        // Reset accumulator
        counter1_value = 0;

        flag1 = 1;
      } else if (flag1 == 2) {
        // Reset counter 1
        TCNT1 = 0;
        flag1 = 0;
      }

    } else {
      flag1 = 2;
    }
  }
}

// Output Heart rate on LCD
// @param bpf_val [float] Filtered Heart signal value
// @return Sends Average Heart Rate to LCD
void bpf_output(float bpf_val) {
  // Wait 1.5 s for stability
  if (counter2_value > 0.3) {

    // Set treshold bpf_value value to 0.5
    if (bpf_val > -55) {
      // Counter 1 Handling
      if  (flag2 == 0) {

        // Show calculated average breath rate on LCD
        sprintf(string, "%.2f |", heart_average(counter2_value));
        lcd_gotoxy(0,1);
        lcd_puts(string);

        // Reset Counter 2
        TCNT2 = 0;

        // Reset accumulator
        counter2_value = 0;

        flag2  = 1;
      } else if (flag2 == 2) {
        // Reset counter 2
        TCNT2 = 0;
        flag2 = 0;
      }

    } else {
      flag2 = 2;
    }
  }
}

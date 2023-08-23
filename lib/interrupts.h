volatile float counter1_value, counter2_value;
volatile float counter1_step = 0.01;
volatile float counter2_step = 0.01 + 0.000000125;

// Counter1 match interrupt handler
// @return Updates counter variable by counter1_step
// @return Resets counter1
interrupt[7] void Counter1_match(void) {
  // Increase accumulator by timer step
  counter1_value += counter1_step;
    // Reset Counter 2
  TCNT1 = 0;
}

// Counter2 match interrupt handler
// @return Updates counter variable by counter2_step
// @return Resets counter2
interrupt[4] void Counter2_match(void) {
  // Increase accumulator by timer step
  counter2_value += counter2_step;

  // Reset Counter 2
  TCNT2 = 0;
}

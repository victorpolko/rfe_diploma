// Setup microcontroller
void initialize(void) {
  DDRB  = 0b11111111; // Port B as output
  PORTB = 0b00000000; // Reset Port B pins

  DDRC  = 0b00000000; // Port C as input
  PORTC = 0b11111111; // Enable pull-up resistors

  DDRD  = 0b11111111; // Port D as output
  PORTD = 0b00000000; // Reset Port D pins

  // Timers/Counters
  // Counter 1 (16 bit) for breath
  // Set timer mask to have interrupts on compare
  TIMSK  |= (1<<OCIE1A);
  // Set prescaler to 1/8 = 250 kHz
  TCCR1B = (0<<CS12) | (1<<CS11) | (0<<CS10);
  // Set comparison value 250000·0.01s = 2500
  OCR1A  = 2500;

  // Counter 2 (8 bit) for heart
  // Set timer mask to have interrupts on compare
  TIMSK  |= (1<<OCIE2);
  // Set prescaler to 1/128 = 15625 Hz
  TCCR2 = (1<<CS22) | (0<<CS21) | (1<<CS20);
  // Set comparison value 15625·0.01s = 156.25
  OCR2   = 156;

  // Allow global interrupts
  SREG |= (1 << SREG_I);

  // Disable comparator for power consumption reduction
  ACSR |= (1 << ACD);

  // Setup ADC
  ADC_init();

  // LCD initialization
  lcd_init(16);

  // Show Parameters on LCD
  lcd_putsf("HR \t\t  | \tBR");

  // Prepare filters
  prepare_lpf();
  prepare_bpf();
}

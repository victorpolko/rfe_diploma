// Initialize ADC unit with appropriate values
// @return Sets ADC as needed
void ADC_init(void) {
  // Set ADC input to ADC0
  ADMUX |= (0 << MUX0)  | // Bits for ADC input
           (0 << MUX1)  | // decimal = binary
           (0 << MUX2)  | // e.g. 0111 = ADC7
           (0 << MUX3)  | // or   0100 = ADC4
           // Unused bit
           (0 << ADLAR) | // Left adjust result (1 - crop lower bits)
           (0 << REFS0) | //
           (0 << REFS1);  //

  ADCSRA |= (0 << ADPS0) | // ADC Prescaler select bits
            (0 << ADPS1) | // 000     CPU/2
            (1 << ADPS2) | // 001     CPU/2
                           // 010     CPU/4
                           // 011     CPU/8
                           // 100     CPU/16
                           // 101     CPU/32
                           // 110     CPU/64
                           // 111     CPU/128
                           // 100 - Quantize 2MHz/16 = 125kHz
            (0 << ADIE)  | // ADC Interrupts permission
            (0 << ADIF)  | // ADC Interrupt flag
            (0 << ADFR)  | // ADC Free running select
            (0 << ADSC)  | // ADC Start Conversion
            (1 << ADEN);   // ADC Enable
}

// AD conversion result
// @param adc_input [unsigned char] ADC channel to be listened to
// @return [int] Conversion result
unsigned int ADC_result(unsigned char adc_input) {
  // Set ADC channel to selected
  ADMUX = adc_input | (ADMUX & 0b11110000);

  delay_us(10);

  // Start conversion
  ADCSRA |= (1<<ADSC);

  // Wait until it's done
  while((ADCSRA & ADIF) == 0);

  // Return ADCH and ADCL in order we specify with ADLAR
  return ADCW;
}

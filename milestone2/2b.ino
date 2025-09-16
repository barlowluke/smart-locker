void setup() {
  // Set up Timer1 for phase-correct PWM at 50Hz
  TCCR1A = 0;  // Reset Timer1 control register A
  TCCR1B = 0;  // Reset Timer1 control register B
  
  // Set phase-correct PWM mode (Mode 10: PWM, Phase Correct, 10-bit)
  TCCR1A |= (1 << WGM11) | (1 << COM1A1);  // Clear OC1A on compare match
  TCCR1B |= (1 << WGM13) | (1 << CS11);    // Prescaler = 8
  
  // Calculate values for 50Hz (20ms period)
  // CPU frequency: 16MHz, Prescaler: 8
  // Timer frequency = 16MHz / 8 = 2MHz
  // Period = 20ms = 0.02s
  // ICR1 = (0.02s * 2,000,000Hz) / 2 = 20,000 (phase-correct divides by 2)
  ICR1 = 20000;  // Set top value for 50Hz
  
  // Calculate pulse width for 1.5ms high
  // Pulse width = (1.5ms * 2,000,000Hz) / 1000 = 3000
  OCR1A = 3000;  // Set compare value for 1.5ms pulse
  
  // Set OC1A (Arduino pin 9) as output
  DDRB |= (1 << PB1);  // PB1 = Arduino digital pin 9
}

void loop() {
  // Calculate and print duty cycle information
  // float pulse_width_ms = 1.5;  // Convert timer ticks to ms
  // float period_ms = 20.0;  // Fixed 20ms period for 50Hz
  // float duty_cycle_percent = (pulse_width_ms / period_ms) * 100.0;
  
  // Serial.print("Pulse Width: ");
  // Serial.print(pulse_width_ms, 2);
  // Serial.print("ms (");
  // Serial.print(duty_cycle_percent, 1);
  // Serial.print("%) | OCR1A: ");
  // Serial.println(OCR1A);

  delay(500);
}

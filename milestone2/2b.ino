int getValidAngle() {
  int angle;
  bool validInput = false;
  while (!validInput) {
    Serial.println();
    Serial.println("Please enter an angle (-90, 0, or 90):");
    while (!Serial.available() > 0) {
      delay(10);
    }
    String input = Serial.readString();
    input.trim();
    if (input == "0") {
      angle = 0;
    } else if (input.toInt() == 0) {
      angle = -1;
    } else {
      angle = input.toInt();
    }
    Serial.print("Raw input: ");
    Serial.println(angle);
    // validate
    if (angle == -90 || angle == 0 || angle == 90) {
      validInput = true;
      Serial.print("Valid input received: ");
      Serial.println(angle);
    } else {
      Serial.print("Invalid input: '");
      Serial.print(angle);
      Serial.println("'. Please enter -90, 0, or 90.");
    }
  }
  while (Serial.available()) {
    Serial.read();
  }
  return angle;
}

void setup() {
  Serial.begin(9600);

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
}

void loop() {
  int chosenAngle = getValidAngle();
  // duration of pulse, in milliseconds
  float high = 0;
  switch (chosenAngle) {
    case -90:
      high = 1;
    case 0:
      high = 1.5;
    case 90:
      high = 2;
  }
  // Calculate pulse width for high duration
  // Pulse width = (high * 2,000,000Hz) / 1000 = 3000
  OCR1A = (high * 2000000) / 1000;  // Set compare value for 1.5ms pulse
  Serial.print("OCR1A: ");
  Serial.println(OCR1A);

  // Calculate and print duty cycle information
  float pulse_width_ms = high;  // Convert timer ticks to ms
  float period_ms = 20.0;  // Fixed 20ms period for 50Hz
  float duty_cycle_percent = (pulse_width_ms / period_ms) * 100.0;
  
  Serial.print("Pulse Width: ");
  Serial.print(pulse_width_ms, 2);
  Serial.print("ms (");
  Serial.print(duty_cycle_percent, 1);
  Serial.print("%) | OCR1A: ");
  Serial.println(OCR1A);

  delay(1000);
}
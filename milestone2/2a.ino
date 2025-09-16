const int colPins[3] = {A0, A1, A2};  // analog inputs for columns

// ADC values for your measured row voltages
const int rowValues[4] = {93, 179, 320, 512};
const int tolerance = 50;  // tolerance range that doesn't overlap

// Key mapping
char keys[4][3] = {{'1', '2', '3'},{'4', '5', '6'},{'7', '8', '9'},{'*', '0', '#'}};

char lastKey = '\0';  // stores the last key detected

void setup() {
  Serial.begin(9600);
}

void loop() {
  char key = getKey();

  if (key != lastKey) 
  {
            // checks if the key has been unpressed
    if (key != '\0') 
    {         // and it’s a valid key (not released)
      Serial.println(key);     // prints character
    }
    lastKey = key;             // update last key state
  }
}

char getKey() {
  for (int col = 0; col < 3; col++) 
  {
    int analogValue = analogRead(colPins[col]);
    int row = decodeRow(analogValue);

    if (row != -1) 
    {
      return keys[row][col];
    }
  }
  return '\0';  // no key pressed
}

int decodeRow(int value) 
{
  for (int r = 0; r < 4; r++) 
  {
    if (abs(value - rowValues[r]) < tolerance) 
    {
      return r;
    }
  }
  return -1;
}

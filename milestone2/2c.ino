enum SystemState {
  STATE_LOCKED,
  STATE_LOCKING,
  STATE_UNLOCKED
};

const char PASSWORD[] = "1234";  // Default password
char inputBuffer[6] = "";        // Buffer for user input (4 digits + null terminator)
int inputIndex = 0;

SystemState currentState = STATE_LOCKED;
unsigned long lockTimer = 0;
const unsigned long LOCK_DELAY = 2000; // 2 seconds delay before locking
bool errorDisplayed = false;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println("=== PASSWORD SYSTEM ===");
  printSystemState();
  Serial.println("Enter 4-digit password followed by # to submit:");
}

void loop() {
  // Process based on current state
  switch (currentState) {
    
    case STATE_LOCKED:
      handleLockedState();
      break;
      
    case STATE_LOCKING:
      handleLockingState();
      break;
      
    case STATE_UNLOCKED:
      handleUnlockedState();
      break;
  }
  
  delay(50);
}

void handleLockedState() {
  if (Serial.available() > 0) {
    char key = Serial.read();
    
    // reset error flag when a new input starts
    if (key >= '0' && key <= '9' || key == '#' || key == '\b') {
      errorDisplayed = false;
    }

    // Process numeric digits
    if (key >= '0' && key <= '9') {
      // Check if buffer has space for more digits
      if (inputIndex < 4) {
        inputBuffer[inputIndex++] = key;
        inputBuffer[inputIndex] = '\0';
        Serial.print("*"); // Show input feedback
      } else {
        // Buffer full - ignore additional digits
        if (!errorDisplayed) {
          Serial.print("errorDisplayed: ");
          Serial.println(errorDisplayed);
          Serial.println("\nError: Password too long! Maximum 4 digits.");
          Serial.println("Please enter # to submit or backspace to correct:");
          errorDisplayed = true;
        }
        inputIndex = 5;
      }
    }
    // Handle submit (# symbol)
    else if (key == '#') {
      processPasswordSubmission();
    }
    // Handle clear/backspace
    else if (key == '\b' || key == 8) { // Backspace or delete
      if (inputIndex > 0) {
        inputIndex--;
        inputBuffer[inputIndex] = '\0';
        Serial.print("\b \b"); // Clear the character visually
      }
    }
    // Handle enter/return (also accept as submit for convenience)
    else if (key == '\n' || key == '\r') {
      processPasswordSubmission();
    }
    // Ignore other non-numeric characters
    else if (key != '\n' && key != '\r') {
      if (!errorDisplayed) {
        Serial.println("\nInvalid character! Only digits 0-9 and # are accepted.");
        Serial.print("Current input: ");
        for (int i = 0; i < inputIndex; i++) Serial.print("*");
        Serial.println();
        errorDisplayed = true;
      }
    }
  }
}

void processPasswordSubmission() {
  Serial.println(); // New line after submission
  errorDisplayed = false;

  if (inputIndex == 0) {
    Serial.println("No password entered!");
    Serial.println("Enter 4-digit password followed by #:");
    errorDisplayed = true;
    return;
  }

  if (inputIndex > 4) {
    Serial.print("inputIndex: ");
    Serial.println(inputIndex);
    Serial.println("Error: Password too long! Maximum 4 digits allowed.");
    Serial.println("Please start over.");
    errorDisplayed = true;
    resetInput();
    return;
  }

  if (inputIndex < 4) {
    Serial.print("Error: Password too short! Expected 4 digits, got ");
    Serial.print(inputIndex);
    Serial.println(" digits.");
    Serial.println("Please enter exactly 4 digits followed by #:");
    errorDisplayed = true;
    resetInput();
    return;
  }
  
  checkPassword();
}

void handleLockingState() {
  // Check if locking delay has passed
  unsigned long currentTime = millis();
  
  // Show locking progress every 500ms
  static unsigned long lastProgressTime = 0;
  if (currentTime - lastProgressTime >= 500) {
    lastProgressTime = currentTime;
    Serial.print(".");
  }
  
  if (currentTime - lockTimer >= LOCK_DELAY) {
    currentState = STATE_LOCKED;
    resetInput();
    Serial.println("\nSystem locked.");
    printSystemState();
    Serial.println("Enter 4-digit password followed by # to submit:");
  }
}

void handleUnlockedState() {
  // Check for lock command via serial
  if (Serial.available() > 0) {
    char command = Serial.read();
    
    if (command == 'L' || command == 'l') {
      // Start locking process
      currentState = STATE_LOCKING;
      lockTimer = millis();
      Serial.println("\nLocking system...");
    }
    else if (command == 'H' || command == 'h') {
      Serial.println("\n=== HELP ===");
      Serial.println("Enter 'L' to lock the system");
      Serial.println("Enter 'H' for help");
      Serial.println("Enter 'S' for system status");
      Serial.println("Enter 'C' to clear screen");
    }
    else if (command == 'S' || command == 's') {
      printSystemState();
    }
    else if (command == 'C' || command == 'c') {
      Serial.println("\n\n\n\n\n\n\n\n\n\n"); // Clear screen approximation
      printWelcomeMessage();
    }
    // Ignore other characters to prevent accidental locking
    else if (command != '\n' && command != '\r') {
      Serial.println("Unknown command. Type 'H' for help.");
    }
  }
}

void checkPassword() {
  if (strcmp(inputBuffer, PASSWORD) == 0) {
    // Correct password
    currentState = STATE_UNLOCKED;
    Serial.println("Password CORRECT - System UNLOCKED");
    printSystemState();
    Serial.println("Enter 'L' to lock the system");
    Serial.println("Enter 'H' for help");
  } else {
    // Incorrect password
    Serial.println("Password INCORRECT - Try again");
    resetInput();
    Serial.println("Enter 4-digit password followed by # to submit:");
  }
}

void resetInput() {
  inputIndex = 0;
  inputBuffer[0] = '\0';
}

void printSystemState() {
  Serial.print("Current state: ");
  switch (currentState) {
    case STATE_LOCKED:
      Serial.println("LOCKED");
      break;
    case STATE_LOCKING:
      Serial.println("LOCKING (in progress)");
      break;
    case STATE_UNLOCKED:
      Serial.println("UNLOCKED");
      break;
  }
}

void printWelcomeMessage() {
  Serial.println("=== PASSWORD SYSTEM ===");
  printSystemState();
  Serial.println("\n=== COMMANDS ===");
  Serial.println("LOCKED state:");
  Serial.println("  - Enter 4-digit password followed by #");
  Serial.println("  - Use backspace to delete characters");
  Serial.println("  - Press Enter or # to submit");
  Serial.println("");
  Serial.println("UNLOCKED state:");
  Serial.println("  'L' - Lock the system");
  Serial.println("  'H' - Show help");
  Serial.println("  'S' - Show system status");
  Serial.println("  'C' - Clear screen");
  Serial.println("================================");
}
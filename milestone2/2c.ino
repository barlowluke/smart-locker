enum States {
  INPUT_PASSWORD, CHECK_PASSWORD, UNLOCKED, LOCKED, ERROR
};

States current_state = LOCKED;
String saved_password = "1234";
String input_password = "";
String button = "";

void input_states() {
  switch(current_state) {
    case INPUT_PASSWORD:
      Serial.println("Enter the first button:");
      if (button == "#") { // # means submit
        current_state = CHECK_PASSWORD;
      } else if (button == "*") { // * means lock
        current_state = LOCKED;
      } else {
        input_password = input_password + button;
      }
      break;

    case CHECK_PASSWORD:
      print_state(current_state);
      if (input_password == saved_password) {
        current_state = UNLOCKED;
      } else {
        current_state = LOCKED;
      }
      break;

    case UNLOCKED:
      print_state(current_state);
      break;

    case LOCKED:
      // intentional fallthrough
      print_state(current_state);
      break;

    case ERROR:
      print_state(current_state);    
      // delay as temporary replacement of buzzer
      delay(3000);
      Serial.println("Error!");
      break;
  }
}

void handleSerialCommand(String command) {
  if (current_state == INPUT_PASSWORD) {
    if (command == "clear") {
      input_password = "";
      Serial.println("Input cleared");
    } else if (command == "enter") {
      if (input_password.length() == 4) {
        current_state = CHECK_PASSWORD;
        Serial.println("Checking password...");
      } else {
        Serial.println("Please enter a password first");
      }
    } else if (command.length() == 1 && isDigit(command[0])) {
      if (input_password.length() < 4) {
        input_password += command;
        Serial.println("* added to password");
      } else {
        Serial.println("Maximum password length reached (4 digits)");
      }
    } else {
      Serial.println("Invalid input. Enter digits 0-9, 'clear', or 'enter'");
    }
  }
}

void setLockedState() {
  current_state = LOCKED;
  input_password = "";
  Serial.println("System locked");
}

void setInputPasswordState() {
  current_state = INPUT_PASSWORD;
  Serial.println("Enter password via serial");
  Serial.println("Send digits 0-9, 'clear', or 'enter'");
}

void setUnlockedState() {
  current_state = UNLOCKED;
  Serial.println("Password correct! System unlocked");
}

void setErrorState() {
  current_state = ERROR;
  Serial.println("ERROR: Incorrect password!");
}

void print_state(States current_state) {
  Serial.print("Current state: ");
  switch (current_state) {
    case LOCKED: Serial.println("LOCKED"); break;
    case INPUT_PASSWORD: Serial.println("INPUT_PASSWORD"); break;
    case CHECK_PASSWORD: Serial.println("CHECK_PASSWORD"); break;
    case UNLOCKED: Serial.println("UNLOCKED"); break;
    case ERROR: Serial.println("ERROR"); break;
  }
  Serial.println("Press password button to begin input");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Starting...");
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    Serial.println(command);
  }
  input_states();
}

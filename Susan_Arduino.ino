// Define motor control pins:
#define stp 12
#define dir 13 
#define EN  11

// Define Limit Switch and reset button:
#define limit_switch 10
#define RESET_BUTTON 4

// Define LED pins:
#define SLOT0 6
#define SLOT1 7
#define SLOT2 8
#define SLOT3 9

// Define direction as boolean:
#define CLOCKWISE 0
#define COUNTERCLOCKWISE 1

// Global Variables:
int current_pos;
int desired_pos;
int desired_LED;
bool limit; // Limit Switch
int x; // For loops

void setup() {

  Serial.begin(57600);

  // Set limit switch and reset button as input:
  pinMode(7, INPUT);
  pinMode(4, INPUT);

  // Set LEDs as output:
  pinMode(SLOT0, OUTPUT);
  pinMode(SLOT1, OUTPUT);
  pinMode(SLOT2, OUTPUT);
  pinMode(SLOT3, OUTPUT);

  // Set motor controls as output:
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(EN, OUTPUT);

  resetEDPins(); // Set all motor control pins to default

           // Physically rotate to position 0:
  current_pos = 0;
}

void loop() {
  
  while (Serial.available()) {

    String consolidate = Serial.readString();
    char firstchar = consolidate.charAt(0);
    char secondchar = consolidate.charAt(1);
    desired_pos = (int)firstchar - 48;
    desired_LED = (int)secondchar - 48;
    //  Serial.read(); // discard new line char
    //scanf("%d%d", &desired_pos, &desired_LED);

    digitalWrite(EN, LOW); // Enable motor control

    Serial.print("Current position is: ");
    Serial.println(current_pos);

    Serial.print("Desired Position is: ");
    Serial.println(desired_pos);

    Serial.print("Desired LED is: ");
    Serial.println(desired_LED);

    // Run motor algorithm:
    find_next_pos();

    delay(1000); // Please wait before crossing the street

           // Turn on appropriate LED:
    set_LED();
    resetEDPins();

    digitalWrite(EN, HIGH); // Disable for safety
  }
}

int ASCII_to_decimal(char x) {
  int decimal = x - 48;
  return decimal;
}

void turnOffAllLEDs() {
  // Turn off all LEDs:
    digitalWrite(SLOT0, LOW);
    digitalWrite(SLOT1, LOW);
    digitalWrite(SLOT2, LOW);
    digitalWrite(SLOT3, LOW);
}

void rotate(bool direction) {

  digitalWrite(dir, direction);
  if (direction) {
    dec_pos();
  }
  else {
    inc_pos();
  }

  for (x = 0; x < 400; x++) {
    digitalWrite(stp, HIGH); // Trigger one step forward
    delay(2);
    digitalWrite(stp, LOW); // Pull step pin low so it can be triggered again
    delay(2);
  }
  limit = digitalRead(limit_switch);
  while (!limit) {
    digitalWrite(stp, HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp, LOW); //Pull step pin low so it can be triggered again
    delay(1);
    limit = digitalRead(limit_switch);
  }
}

//Reset Easy Driver pins to default states
void resetEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(EN, HIGH);
}

void find_next_pos() {

  turnOffAllLEDs();

  if (current_pos == 0) {
    if (desired_pos == 0) {
      // DO NOTHING
    }
    else if (desired_pos == 1) {
      rotate(CLOCKWISE);
    }
    else if (desired_pos == 2) {
      rotate(CLOCKWISE);
      rotate(CLOCKWISE);
    }
    else if (desired_pos == 3) {
      rotate(COUNTERCLOCKWISE);
    }
  }
  else if (current_pos == 1) {
    if (desired_pos == 0) {
      rotate(COUNTERCLOCKWISE);
    }
    else if (desired_pos == 1) {
      // DO NOTHING
    }
    else if (desired_pos == 2) {
      rotate(CLOCKWISE);
    }
    else if (desired_pos == 3) {
      rotate(COUNTERCLOCKWISE);
      rotate(COUNTERCLOCKWISE);
    }
  }
  else if (current_pos == 2) {
    if (desired_pos == 0) {
      rotate(COUNTERCLOCKWISE);
      rotate(COUNTERCLOCKWISE);
    }
    else if (desired_pos == 1) {
      rotate(COUNTERCLOCKWISE);
    }
    else if (desired_pos == 2) {
      // DO NOTHING
    }
    else if (desired_pos == 3) {
      rotate(CLOCKWISE);
    }
  }
  else if (current_pos == 3) {
    if (desired_pos == 0) {
      rotate(CLOCKWISE);
    }
    else if (desired_pos == 1) {
      rotate(CLOCKWISE);
      rotate(CLOCKWISE);
    }
    else if (desired_pos == 2) {
      rotate(COUNTERCLOCKWISE);
    }
    else if (desired_pos == 3) {
      // DO NOTHING
    }
  }
}

void inc_pos() {
  if (current_pos == 3) // max
    current_pos = 0;
  else
    current_pos++;
}

void dec_pos() {
  if (current_pos) // nonzero
    current_pos--;
  else
    current_pos = 3;
}

void set_LED() {

  if (desired_LED == 0) {
    digitalWrite(SLOT0, HIGH);
  }

  else if (desired_LED == 1) {
    digitalWrite(SLOT1, HIGH);
  }

  else if (desired_LED == 2) {
    digitalWrite(SLOT2, HIGH);
  }

  else if (desired_LED == 3) {
    digitalWrite(SLOT3, HIGH);
  }

}

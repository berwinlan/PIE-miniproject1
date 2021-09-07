

const uint8_t button_interval = 50;
uint32_t button_time;

// Say which component is connect to what pin
const uint8_t LED9 = 9;
const uint8_t LED10 = 10;
const uint8_t LED11 = 11;
const uint8_t LED12 = 12;
const uint8_t LED13 = 13;
const uint8_t SW = 8;

enum states {
  NONE,
  ALL_ON,
  ALL_OFF,
  ALL_BLINKING,
  TRAVELING,
  SOS
};

states current_state, prior_state;

void all_on() {
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;

    // Set all LEDs to HIGH
    digitalWrite(LED9, HIGH);
    digitalWrite(LED10, HIGH);
    digitalWrite(LED11, HIGH);
    digitalWrite(LED12, HIGH);
    digitalWrite(LED13, HIGH);
  }


  // No state tasks

  // Check for state
  if (digitalRead(SW) == HIGH) {
    current_state = ALL_OFF; 
  }

//  // Clean up if leaving
//  if (current_state != prior_state) {
//    prior_state = current_state;
//
//    // Set all LEDs to LOW
//    digitalWrite(LED9, LOW);
//    digitalWrite(LED10, LOW);
//    digitalWrite(LED11, LOW);
//    digitalWrite(LED12, LOW);
//    digitalWrite(LED13, LOW);
//  }
}

void all_off() {
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;

    // Set all LEDs to LOW
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
  }

  // No state tasks

  // Check for state
  if (digitalRead(SW) == HIGH) {
    current_state = ALL_ON; 
    digitalWrite(SW, LOW);
  }

//  // Clean up if leaving
//  if (current_state != prior_state) {
//    prior_state = current_state;
//
//    // Set all LEDs to LOW
//    digitalWrite(LED9, HIGH);
//    digitalWrite(LED10, LOW);
//    digitalWrite(LED11, LOW);
//    digitalWrite(LED12, LOW);
//    digitalWrite(LED13, LOW);
//  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED9, OUTPUT);
  pinMode(LED10, OUTPUT);
  pinMode(LED11, OUTPUT);
  pinMode(LED12, OUTPUT);
  pinMode(LED13, OUTPUT);
  pinMode(SW, INPUT);

  digitalWrite(LED9, LOW);
  digitalWrite(LED10, LOW);
  digitalWrite(LED11, LOW);
  digitalWrite(LED12, LOW);
  digitalWrite(LED13, LOW);

  prior_state = NONE;
  current_state = ALL_ON;

  button_time = millis();  // time when starting
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t current_time;  // local variable for current time

  current_time = millis();
  if (current_time > button_time + button_interval) {
      Serial.println(current_state);
      switch (current_state) {
        case ALL_ON:
          all_on();
          break;
        case ALL_OFF:
          all_off();
          break;
      }
  }
}

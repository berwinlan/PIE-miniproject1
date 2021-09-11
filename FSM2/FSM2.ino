
// Say which component is connect to what pin
const uint8_t LED9 = 9;
const uint8_t LED10 = 10;
const uint8_t LED11 = 11;
const uint8_t LED12 = 12;
const uint8_t LED13 = 13;
const uint8_t SW = 8;

// For debouncing
const uint8_t DEBOUNCE_INTERVAL = 10; // ms
uint32_t debounce_time;
bool SW_went_back_low;  // track that button has finished

enum states {                         // Define enumerated type for state machine states
  NONE,
  ALL_ON,
  ALL_OFF,
  ALL_BLINKING,
  TRAVELING,
  SOS
};

states current_state, prior_state;

uint32_t blink_time;                 // Global variable to store the time that LEDs last changed state
const uint16_t BLINK_INTERVAL = 500;  // Time interval between toggling blinking LEDs in milliseconds

uint32_t travel_time;
const uint16_t TRAVEL_INTERVAL_LED9 = 500;
const uint16_t TRAVEL_INTERVAL_LED10 = 1000;
const uint16_t TRAVEL_INTERVAL_LED11 = 1500;
const uint16_t TRAVEL_INTERVAL_LED12 = 2000;
const uint16_t TRAVEL_INTERVAL_LED13 = 2500;

void all_on() {
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;
    digitalWrite(LED9, HIGH);
    digitalWrite(LED10, HIGH);
    digitalWrite(LED11, HIGH);
    digitalWrite(LED12, HIGH);
    digitalWrite(LED13, HIGH);

//    analogWrite(LED9, outputValue);
//    analogWrite(LED10, outputValue);
//    analogWrite(LED11, outputValue);
//    analogWrite(LED12, outputValue);
//    analogWrite(LED13, outputValue);
  }

  // Perform state tasks
//  Serial.println("ALL_ON state task");

  uint32_t t;                           // Local variable to store the current value of the millis timer
  bool SW_high;                        // Local variable to store whether SW is high

  t = millis();                         // Get the current value of the millis timer
  
  // Check for state transitions
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    SW_high = digitalRead(SW) == HIGH;
    if (SW_went_back_low && SW_high) {
      current_state = ALL_OFF;
      SW_went_back_low = false;
    } else if (!SW_went_back_low && !SW_high) {
      SW_went_back_low = true;
    }
    debounce_time = t;
  }

  if (current_state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
  }
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
//  Serial.println("ALL_OFF state task");

  uint32_t t;                           // Local variable to store the current value of the millis timer
  bool SW_high;                        // Local variable to store whether SW is high

  t = millis();                         // Get the current value of the millis timer

  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    SW_high = digitalRead(SW) == HIGH;
    if (SW_went_back_low && SW_high) {
      current_state = ALL_BLINKING;
      SW_went_back_low = false;
    } else if (!SW_went_back_low && !SW_high) {
      SW_went_back_low = true;
    }
    debounce_time = t;
  }

  // Clean up if leaving
  if (current_state != prior_state) {
    // Set all LEDs to LOW
    digitalWrite(LED9, HIGH);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
  }
}

void all_blinking() {
  uint32_t t;
  
  if (current_state != prior_state) {
    prior_state = current_state;
    
    // Set all LEDs to LOW
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);

    blink_time = millis();
  }

  // Perform state tasks
//  Serial.println("BLINKING state task");
  t = millis();
  if (t >= blink_time + BLINK_INTERVAL) {
    digitalWrite(LED9, !digitalRead(LED9));
    digitalWrite(LED10, !digitalRead(LED10));
    digitalWrite(LED11, !digitalRead(LED11));
    digitalWrite(LED12, !digitalRead(LED12));
    digitalWrite(LED13, !digitalRead(LED13));
    blink_time = t;
  }

  bool SW_high;                        // Local variable to store whether SW is high

  t = millis();                         // Get the current value of the millis timer

  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    SW_high = digitalRead(SW) == HIGH;
    if (SW_went_back_low && SW_high) {
      current_state = TRAVELING;
      SW_went_back_low = false;
    } else if (!SW_went_back_low && !SW_high) {
      SW_went_back_low = true;
    }
    debounce_time = t;
  }

  if (current_state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
  }
}

void traveling() {
  uint32_t t;
  uint16_t TRAVEL_INTERVAL = 300; // length of time each light is on (ms)
  
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;
    
    // Set first LED to HIGH and rest to LOW
    digitalWrite(LED9, HIGH);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
    
    travel_time = millis();
  }

  // Perform state tasks
  t = millis();
  if (t >= travel_time + TRAVEL_INTERVAL_LED9) {
    digitalWrite(LED9, !digitalRead(LED9));
    digitalWrite(LED10, !digitalRead(LED10));
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED10) {
    digitalWrite(LED10, !digitalRead(LED10));
    digitalWrite(LED11, !digitalRead(LED11));
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED11) {
    digitalWrite(LED11, !digitalRead(LED11));
    digitalWrite(LED12, !digitalRead(LED12));
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED12) {
    digitalWrite(LED12, !digitalRead(LED12));
    digitalWrite(LED13, !digitalRead(LED13));
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED13) {
    digitalWrite(LED13, !digitalRead(LED13));
    digitalWrite(LED9, !digitalRead(LED9));
    travel_time = t;
  }
 
//  // Perform state tasks
//  for (int i = LED9; i <= LED13; i++) {
//    delay(TRAVEL_INTERVAL);
//    digitalWrite(i, !digitalRead(i));
//    digitalWrite(i+1, !digitalRead(i+1));
//  }
//  digitalWrite(LED9, HIGH);

  bool SW_high;                        // Local variable to store whether SW is high
  t = millis();                         // Get the current value of the millis timer

  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    SW_high = digitalRead(SW) == HIGH;
    if (SW_went_back_low && SW_high) {
      current_state = ALL_ON;
      SW_went_back_low = false;
    } else if (!SW_went_back_low && !SW_high) {
      SW_went_back_low = true;
    }
    debounce_time = t;
  }

  if (current_state != prior_state) {         // If we are leaving the state, do clean up stuff
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
  }
}

void setup() {
  // Configure pins
  pinMode(LED9, OUTPUT);
  pinMode(LED10, OUTPUT);
  pinMode(LED11, OUTPUT);
  pinMode(LED12, OUTPUT);
  pinMode(LED13, OUTPUT);
  pinMode(SW, INPUT);
  
  // Alternating lights = initialization complete
  digitalWrite(LED9, HIGH);
  digitalWrite(LED10, LOW);
  digitalWrite(LED11, HIGH);
  digitalWrite(LED12, LOW);
  digitalWrite(LED13, HIGH);

  prior_state = NONE;
  current_state = ALL_ON;

  blink_time = millis();
  debounce_time = blink_time;
  SW_went_back_low = true;

  Serial.begin(9600);
}

void loop() {
  Serial.print(prior_state);
  Serial.print(" -> ");
  Serial.println(current_state);
  switch (current_state) {
    Serial.print(prior_state);
    Serial.print(" -> ");
    Serial.println(current_state);
    case ALL_ON:
      all_on();
      break;
    case ALL_OFF:
      all_off();
      break;
    case ALL_BLINKING:
      all_blinking();
      break;
    case TRAVELING:
      traveling();
      break;
//     case SOS:
//      blinking_SOS();
//      break;
  }
}

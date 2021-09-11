
// Say which component is connected to what pin
const uint8_t LED9 = 9;
const uint8_t LED10 = 10;
const uint8_t LED11 = 11;
const uint8_t LED12 = 12;
const uint8_t LED13 = 13;
const uint8_t SW = 8;

// Variable LED brightness for ALL_ON state
const int POT = A0; // Analog pin pot is attached to
uint16_t sensorValue;        // value read from the pot
uint16_t outputValue;        // value output to the PWM (analog out) - output ceiling, overwrite HIGH

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
const uint16_t TRAVEL_INTERVAL_LED10 = 2*TRAVEL_INTERVAL_LED9;
const uint16_t TRAVEL_INTERVAL_LED11 = 3*TRAVEL_INTERVAL_LED9;
const uint16_t TRAVEL_INTERVAL_LED12 = 4*TRAVEL_INTERVAL_LED9;
const uint16_t TRAVEL_INTERVAL_LED13 = 5*TRAVEL_INTERVAL_LED9;

uint8_t s1_counter;
uint8_t o_counter;
uint8_t s2_counter;
const uint16_t S_INTERVAL = 500;
const uint16_t O_INTERVAL = 1000;

void all_on() {
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;
//    digitalWrite(LED9, HIGH);
//    digitalWrite(LED10, HIGH);
//    digitalWrite(LED11, HIGH);
//    digitalWrite(LED12, HIGH);
//    digitalWrite(LED13, HIGH);

    analogWrite(LED9, outputValue);
    analogWrite(LED10, outputValue);
    analogWrite(LED11, outputValue);
    analogWrite(LED12, outputValue);
    analogWrite(LED13, outputValue);
  }

  // Perform state tasks
  // read the analog in value:
  sensorValue = analogRead(POT);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);

  analogWrite(LED9, outputValue);
  analogWrite(LED10, outputValue);
  analogWrite(LED11, outputValue);
  analogWrite(LED12, outputValue);
  analogWrite(LED13, outputValue);
    
  Serial.println(outputValue);

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

// TODO: LEDs aren't fully turning on and off (see for loop for how it should work)
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
  if (t >= travel_time + TRAVEL_INTERVAL_LED9 && t < travel_time + TRAVEL_INTERVAL_LED10) {
//    Serial.println("interval LED9");
    digitalWrite(LED9, !digitalRead(LED9));
    digitalWrite(LED10, !digitalRead(LED10));
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED10 && t < travel_time + TRAVEL_INTERVAL_LED11) {
//    Serial.println("interval LED10");
    digitalWrite(LED10, !digitalRead(LED10));
    digitalWrite(LED11, !digitalRead(LED11));
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED11 && t < travel_time + TRAVEL_INTERVAL_LED12) {
//    Serial.println("interval LED11");
    digitalWrite(LED11, !digitalRead(LED11));
    digitalWrite(LED12, !digitalRead(LED12));
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED12 && t < travel_time + TRAVEL_INTERVAL_LED13) {
//    Serial.println("interval LED12");
    digitalWrite(LED12, !digitalRead(LED12));
    digitalWrite(LED13, !digitalRead(LED13));
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED13) {
//    Serial.println("interval LED13");
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
      current_state = SOS;
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

// TODO: doesn't pause atm :(
void blinking_SOS() {
  uint32_t t;
  
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;
    
    // Set all LEDs to LOW
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);

    blink_time = millis();

    s1_counter = 0;
    o_counter = 0;
    s2_counter = 0;
  }

  // Perform state tasks
  t = millis();

//SSSSSSSSSSSSSSSSSSS
  if (t >= blink_time + S_INTERVAL && s1_counter < 6) {
    Serial.println("1SSSSSSSS");
//        digitalWrite(LED9, !digitalRead(LED9)); // this and the line below is if we want the "shorts" to be a physically shorter line
        digitalWrite(LED9, LOW);        
        digitalWrite(LED10, !digitalRead(LED10));
        digitalWrite(LED11, !digitalRead(LED11));
        digitalWrite(LED12, !digitalRead(LED12));
//        digitalWrite(LED13, !digitalRead(LED13)); // this and the line below is if we want the "shorts" to be a physically shorter line
        digitalWrite(LED13, LOW);
        blink_time = t;

//        Serial.print("S1 before : ");
//        Serial.println(s1_counter);
        s1_counter++;
//        Serial.print("S1 : ");
//        Serial.println(s1_counter);

//OOOOOOOOOOOOOOOOOOOOOOOOOOOO
  } else if (t >= blink_time + O_INTERVAL && s1_counter == 6) {

    Serial.println("OOOOOOOO");
//        while (three_counter < 6){
    digitalWrite(LED9, !digitalRead(LED9));
    digitalWrite(LED10, !digitalRead(LED10));
    digitalWrite(LED11, !digitalRead(LED11));
    digitalWrite(LED12, !digitalRead(LED12));
    digitalWrite(LED13, !digitalRead(LED13));
    blink_time = t;
    o_counter++;
//        Serial.println("O : ");
//        Serial.println(o_counter);
        
//SSSSSSSSSSSSSSSSSSS
// TODO: This part isn't working properly - changing it to +S_INTERVAL creates
  } else if (t >= blink_time + S_INTERVAL && o_counter == 6) { 
//    Serial.println("2SSSSSSSS");
//        digitalWrite(LED9, !digitalRead(LED9)); // this and the line below is if we want the "shorts" to be a physically shorter line
        digitalWrite(LED9, LOW);        
        digitalWrite(LED10, !digitalRead(LED10));
        digitalWrite(LED11, !digitalRead(LED11));
        digitalWrite(LED12, !digitalRead(LED12));
//        digitalWrite(LED13, !digitalRead(LED13)); // this and the line below is if we want the "shorts" to be a physically shorter line
        digitalWrite(LED13, LOW);
        blink_time = t;
        s2_counter++;
//        Serial.println("S2: ");
//        Serial.println(s2_counter);
  } else if (t > blink_time + O_INTERVAL && s2_counter == 6) {   // pause between SOSes
    // Set all LEDs to LOW
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
  } else if (s1_counter == 6 && o_counter == 6 && s2_counter == 6) {
      s1_counter = 0;
      o_counter = 0;
      s2_counter = 0;
  }

  bool SW_high;                        // Local variable to store whether SW is high
  t = millis();                         // Get the current value of the millis timer

  // Check for state transitions
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
  pinMode(POT, INPUT);
  
//  // Alternating lights = initialization complete
//  digitalWrite(LED9, HIGH);
//  digitalWrite(LED10, LOW);
//  digitalWrite(LED11, HIGH);
//  digitalWrite(LED12, LOW);
//  digitalWrite(LED13, HIGH);

  prior_state = NONE;
  current_state = ALL_ON;

  blink_time = millis();
  debounce_time = blink_time;
  SW_went_back_low = true;

  Serial.begin(9600);
}

void loop() {
//  Serial.print(prior_state);
//  Serial.print(" -> ");
//  Serial.println(current_state);
  uint32_t t;

  t = millis();                         // Get the current value of the millis timer

//  Serial.println(current_state);
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    // read the analog in value:
    sensorValue = analogRead(POT);
    // map it to the range of the analog out:
    outputValue = map(sensorValue, 0, 1023, 0, 255);
  
    // write to the analogOutPins (LED9-LED13)
//    analogWrite(LED9, outputValue);
//    analogWrite(LED10, outputValue);
//    analogWrite(LED11, outputValue);
//    analogWrite(LED12, outputValue);
//    analogWrite(LED13, outputValue);  
      // print the results to the Serial Monitor:
      Serial.print("sensor = ");
      Serial.print(sensorValue);
      Serial.print("\t output = ");
      Serial.println(outputValue);
    
    debounce_time = t;
  }

  switch (current_state) {
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
    case SOS:
      blinking_SOS();
      break;
  }
}

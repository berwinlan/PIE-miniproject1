
// Say which component is connected to what pin
const uint8_t LED9 = 9;
const uint8_t LED10 = 10;
const uint8_t LED11 = 11;
const uint8_t LED12 = 5; //Originally was LED 12, changing it to 5 to allow for analog input
const uint8_t LED13 = 6; //Originally was LED 13, changing it to 6 to allow for analog input
const uint8_t SW = 8;

// Variable LED brightness for ALL_ON state
const int POT = A0;          // Analog pin pot is attached to
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

states current_state, prior_state; // Tracking states

uint32_t blink_time;                  // Global variable to store the time that LEDs last changed state
const uint16_t BLINK_INTERVAL = 500;  // Time interval between toggling blinking LEDs in milliseconds
bool blinkon = false;                 // Status of blinking on or blinking off

uint32_t travel_time;                                         // time for the traveling state
const uint16_t TRAVEL_INTERVAL_LED9 = 500;                    // time on for the first LED in the traveling state (ms)
const uint16_t TRAVEL_INTERVAL_LED10 = 2*TRAVEL_INTERVAL_LED9;// time on for the second LED in the traveling state (ms)
const uint16_t TRAVEL_INTERVAL_LED11 = 3*TRAVEL_INTERVAL_LED9;// time on for the third LED in the traveling state (ms)
const uint16_t TRAVEL_INTERVAL_LED12 = 4*TRAVEL_INTERVAL_LED9;// time on for the fourth LED in the traveling state (ms)
const uint16_t TRAVEL_INTERVAL_LED13 = 5*TRAVEL_INTERVAL_LED9;// time on for the fifth LED in the traveling state (ms)
const uint16_t TRAVEL_INTERVAL_END = 6*TRAVEL_INTERVAL_LED9;  // time for LEDs in the traveling state to reset (ms)

uint8_t s1_counter;   //Counter for the the 3 dots for S in Morse code
uint8_t o_counter;    //Counter for the the 3 dashes for O in Morse code
uint8_t s2_counter;   //Counter for the the 3 dots for S in Morse code
const uint16_t S_INTERVAL = 500;    //length of the Morse code dot
const uint16_t O_INTERVAL = 1000;   //length of the Morse code dashes

void all_low() {
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
   }
   
void singleLED_on(char LEDnum) {
  // read the analog in value:
  sensorValue = analogRead(POT);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
    analogWrite(LEDnum, outputValue);
   }

void all_on() {
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;

    singleLED_on(LED9);
    singleLED_on(LED10);
    singleLED_on(LED11);
    singleLED_on(LED12);
    singleLED_on(LED13);
  }

  // Perform state tasks
    singleLED_on(LED9);
    singleLED_on(LED10);
    singleLED_on(LED11);
    singleLED_on(LED12);
    singleLED_on(LED13);

  uint32_t t;                           // Local variable to store the current value of the millis timer
  bool SW_high;                         // Local variable to store whether SW is high

  t = millis();                         // Get the current value of the millis timer
  
  // Check for state transitions
  // Debounce the switch
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
// if we are leaving the state, turn all LEDs off
  if (current_state != prior_state) {  
    all_low();
  }
}

void all_off() {
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;

    // Set all LEDs to LOW
      all_low();
  }

  // No state tasks

  uint32_t t;                           // Local variable to store the current value of the millis timer
  bool SW_high;                         // Local variable to store whether SW is high

  t = millis();                         // Get the current value of the millis timer

  // Check for state transitions
  // Debounce the switch
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
    all_low();
  }
}

void all_blinking() {
  uint32_t t;
// If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;
    
    // Set all LEDs to LOW
    all_low();
    blink_time = millis();
    blinkon = false;
  }

  // Perform state tasks
  t = millis();
  if (t >= blink_time + BLINK_INTERVAL) {
    if (blinkon == true) {
        singleLED_on(LED9);
        singleLED_on(LED10);
        singleLED_on(LED11);
        singleLED_on(LED12);
        singleLED_on(LED13);
    } else if (blinkon == false) {
        all_low();
    }
        blink_time = t;
        blinkon = !blinkon;
  }

  
  bool SW_high;                        // Local variable to store whether SW is high
  t = millis();                         // Get the current value of the millis timer
  
  // Check for state transitions
  // Debounce the switch
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

// if we are leaving the state, turn all LEDs off
  if (current_state != prior_state) {       
    all_low();
  }
}
 
void traveling() {
  uint32_t t;
  uint16_t TRAVEL_INTERVAL = 300; // length of time each light is on (ms)
  
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;
    
    // Set first LED to HIGH and rest to LOW
    all_off();

    travel_time = millis();  
  }

  // Perform state tasks
  
  t = millis();
  if (t >= travel_time + TRAVEL_INTERVAL_LED9 && t < travel_time + TRAVEL_INTERVAL_LED10) {
        singleLED_on(LED9);
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED10 && t < travel_time + TRAVEL_INTERVAL_LED11) {
        singleLED_on(LED10);
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED11 && t < travel_time + TRAVEL_INTERVAL_LED12) {
        singleLED_on(LED11);
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED12 && t < travel_time + TRAVEL_INTERVAL_LED13) {
        singleLED_on(LED12);
  } else if (t >= travel_time + TRAVEL_INTERVAL_LED13 && t < travel_time + TRAVEL_INTERVAL_END) {
        singleLED_on(LED13);
  } else if (t > travel_time + TRAVEL_INTERVAL_END){
      all_low();
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

  // Check for state transitions
  // Debounce the switch
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

  // if we are leaving the state, turn all LEDs off   
  if (current_state != prior_state) {   
    all_low();
  }
}


void blinking_SOS() {
  uint32_t t;
  
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;
   
    // Set all LEDs to LOW
    all_low();
    blink_time = millis();
    blinkon = false;  
    // start all counters at zero
    s1_counter = 0; 
    o_counter = 0;
    s2_counter = 0;
  }

  // Perform state tasks
  t = millis();

// First Morse code S
// loop through 6 times (turning on an off every 2 cycles)
  if (t >= blink_time + S_INTERVAL && s1_counter < 6) {
      if (blinkon == true) {
        singleLED_on(LED10);
        singleLED_on(LED11);
        singleLED_on(LED12);
    } else if (blinkon == false) {
        all_low();
    }
        blink_time = t;
        blinkon = !blinkon;
        s1_counter++;

// Morse code O
// loop through 6 times (turning on an off every 2 cycles)
  } else if (t >= blink_time + O_INTERVAL && s1_counter == 6) {
  if (blinkon == true) {
        singleLED_on(LED9);
        singleLED_on(LED10);
        singleLED_on(LED11);
        singleLED_on(LED12);
        singleLED_on(LED13);
    } else if (blinkon == false) {
        all_low();
    }
    
    blink_time = t;
    blinkon = !blinkon;
    o_counter++;

        
// Second Morse code S
// loop through 6 times (turning on an off every 2 cycles)
  } else if (t >= blink_time + S_INTERVAL && o_counter == 6) { 
      if (blinkon == true) {
        singleLED_on(LED10);
        singleLED_on(LED11);
        singleLED_on(LED12);
    } else if (blinkon == false) {
        all_low();
    }
        blink_time = t;
        blinkon = !blinkon;
        s2_counter++;
        
  } else if (s1_counter == 6 && o_counter == 6 && s2_counter == 6) {
      s1_counter = 0;
      o_counter = 0;
      s2_counter = 0;
  }

  bool SW_high;                         // Local variable to store whether SW is high
  t = millis();                         // Get the current value of the millis timer

  // Check for state transitions
  // Debounce the switch
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
  
// if we are leaving the state, turn all LEDs off
  if (current_state != prior_state) {  
    all_low();
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

// set the prior state and current state to start
  prior_state = NONE;
  current_state = ALL_ON;

  blink_time = millis();
  debounce_time = blink_time;
  SW_went_back_low = true;

  Serial.begin(9600); //baud rate
}

void loop() {
  uint32_t t;

  t = millis();                         // Get the current value of the millis timer

  // Debounce the potentiometer
  if (t >= debounce_time + DEBOUNCE_INTERVAL) {
    // read the analog in value:
    sensorValue = analogRead(POT);
    // map it to the range of the analog out:
    outputValue = map(sensorValue, 0, 1023, 0, 255);
    debounce_time = t;
  }

// Check current state and call appropriate state function 
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

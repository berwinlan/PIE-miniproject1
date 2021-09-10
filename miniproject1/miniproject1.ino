// TODO:
// figure out the not always working switch problem


const uint8_t DEBOUNCE_INTERVAL = 10; // ms
uint32_t debounce_time;
bool SW_went_back_low;  // track that button has finished

const uint8_t BLINK_INTERVAL = 1000;  // ms
uint32_t blink_time;

uint8_t three_counter = 0;



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
  Serial.println("ALL_ON state task");

  // Check for state
  uint32_t current_time;  // local variable for current time

  if (digitalRead(SW) == HIGH) {
    current_state = ALL_OFF; 
    Serial.print("\nprior state: ");
    Serial.print(prior_state);
    Serial.print("     current state: ");
    Serial.println(current_state);
    Serial.println(current_state == prior_state);
  }

//  // Clean up if leaving
//  if (current_state != prior_state) {
//    current_state = ALL_OFF;
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
    current_state = ALL_BLINKING; 
    Serial.print("\nprior state: ");
    Serial.print(prior_state);
    Serial.print("     current state: ");
    Serial.println(current_state);
    Serial.println(current_state == prior_state);
  }

//  // Clean up if leaving
//  if (current_state != prior_state) {
//    current_state = TRAVELING;
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

void all_blinking() {
  uint32_t t;
  
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;

    // Set alternating LEDs
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);

    blink_time = millis();
//       Serial.println("enter blinky time");
  }

  // State tasks
  t = millis();
  while (t < blink_time + BLINK_INTERVAL*2) {
//    Serial.println("blinky time happening");
    digitalWrite(LED9, !digitalRead(LED9));
    digitalWrite(LED10, !digitalRead(LED10));
    digitalWrite(LED11, !digitalRead(LED11));
    digitalWrite(LED12, !digitalRead(LED12));
    digitalWrite(LED13, !digitalRead(LED13));
    blink_time = t;
    delay(BLINK_INTERVAL);
    if (digitalRead(SW) == HIGH) {
          digitalWrite(LED9, LOW);  
          digitalWrite(LED10, LOW);
          digitalWrite(LED11, LOW);
          digitalWrite(LED12, LOW);
          digitalWrite(LED13, LOW);        
      break;
    }
  }

  // Check for state transition
  if (digitalRead(SW) == HIGH) {
    current_state = TRAVELING; 
    Serial.print("\nprior state: ");
    Serial.print(prior_state);
    Serial.print("     current state: ");
    Serial.println(current_state);
    Serial.println(current_state == prior_state);
  }

//  // Clean up if leaving
//  if (current_state != prior_state) {
//    current_state = ALL_OFF;
//
//    // Set all LEDs to LOW
//    digitalWrite(LED9, LOW);
//    digitalWrite(LED10, LOW);
//    digitalWrite(LED11, LOW);
//    digitalWrite(LED12, LOW);
//    digitalWrite(LED13, LOW);
//  }
}

void traveling() {
  uint32_t t;
  uint16_t TRAVEL_INTERVAL = 300; // length of time each light is on (ms)
  
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;

    // Set alternating LEDs
    digitalWrite(LED9, HIGH);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);
  }

  // State tasks
  t = millis();
  while (true){
      for (int i = LED9; i <= LED13; i++) {
        delay(TRAVEL_INTERVAL);
        digitalWrite(i, !digitalRead(i));
        digitalWrite(i+1, !digitalRead(i+1));
        
      }
        digitalWrite(LED9, HIGH);
      if (digitalRead(SW) == HIGH) {
          digitalWrite(LED9, LOW);  
          digitalWrite(LED10, LOW);
          digitalWrite(LED11, LOW);
          digitalWrite(LED12, LOW);
          digitalWrite(LED13, LOW);        
      break;
    }
  }

  // Check for state transition
  // I think there's an infinite loop here? maybe? -BL
  if (digitalRead(SW) == HIGH) {
    current_state = SOS; 
    Serial.print("\nprior state: ");
    Serial.print(prior_state);
    Serial.print("     current state: ");
    Serial.println(current_state);
    Serial.println(current_state == prior_state);
  }

//  // Clean up if leaving
//  if (current_state != prior_state) {
//    current_state = ALL_OFF;
//
//    // Set all LEDs to LOW
//    digitalWrite(LED9, LOW);
//    digitalWrite(LED10, LOW);
//    digitalWrite(LED11, LOW);
//    digitalWrite(LED12, LOW);
//    digitalWrite(LED13, LOW);
//  }
}


void blinking_SOS() {
  uint32_t t;
  
  // If we are entering the state, check for state transition and initialize
  if (current_state != prior_state) {
    prior_state = current_state;

    // Set alternating LEDs
    digitalWrite(LED9, LOW);
    digitalWrite(LED10, LOW);
    digitalWrite(LED11, LOW);
    digitalWrite(LED12, LOW);
    digitalWrite(LED13, LOW);

    blink_time = millis();
//       Serial.println("enter blinky time");
  }

  // State tasks
  t = millis();
  while (t < blink_time + BLINK_INTERVAL*4) {


//SSSSSSSSSSSSSSSSSSS
       three_counter = 0;
      while (three_counter < 6){
        digitalWrite(LED9, LOW);        
        digitalWrite(LED10, !digitalRead(LED10));
        digitalWrite(LED11, !digitalRead(LED11));
        digitalWrite(LED12, !digitalRead(LED12));
        digitalWrite(LED13, LOW);
        blink_time = t;
        delay(BLINK_INTERVAL*2);
        three_counter += 1;
//        Serial.println("S1 : ");
//        Serial.println(three_counter);
      }
        
//OOOOOOOOOOOOOOOOOOOOOOOOOOOO
        three_counter = 0;
        while (three_counter < 6){
        digitalWrite(LED9, !digitalRead(LED9));
        digitalWrite(LED10, !digitalRead(LED10));
        digitalWrite(LED11, !digitalRead(LED11));
        digitalWrite(LED12, !digitalRead(LED12));
        digitalWrite(LED13, !digitalRead(LED13));
        blink_time = t;
        delay(BLINK_INTERVAL*4);
        three_counter += 1;
//        Serial.println("O : ");
//        Serial.println(three_counter);
      }

//SSSSSSSSSSSSSSSSSSS
        three_counter = 0;
      while (three_counter < 6){
        digitalWrite(LED9, LOW);        
        digitalWrite(LED10, !digitalRead(LED10));
        digitalWrite(LED11, !digitalRead(LED11));
        digitalWrite(LED12, !digitalRead(LED12));
        digitalWrite(LED13, LOW);
        blink_time = t;
        delay(BLINK_INTERVAL*2);
        three_counter += 1;
//        Serial.println("S2: ");
//        Serial.println(three_counter);
      }
        delay(BLINK_INTERVAL*3);
      
    if (digitalRead(SW) == HIGH) {
          digitalWrite(LED9, LOW);  
          digitalWrite(LED10, LOW);
          digitalWrite(LED11, LOW);
          digitalWrite(LED12, LOW);
          digitalWrite(LED13, LOW);      
      break;
    }
  }

  // Check for state transition
  if (digitalRead(SW) == HIGH) {
    current_state = ALL_ON; 
    Serial.print("\nprior state: ");
    Serial.print(prior_state);
    Serial.print("     current state: ");
    Serial.println(current_state);
    Serial.println(current_state == prior_state);
  }

//  // Clean up if leaving
//  if (current_state != prior_state) {
//    current_state = ALL_OFF;
//
//    // Set all LEDs to LOW
//    digitalWrite(LED9, LOW);
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

  // Alternating lights = initialization complete
  digitalWrite(LED9, HIGH);
  digitalWrite(LED10, LOW);
  digitalWrite(LED11, HIGH);
  digitalWrite(LED12, LOW);
  digitalWrite(LED13, HIGH);

  prior_state = NONE;
  current_state = ALL_ON;

  debounce_time = millis();  // time when starting
  SW_went_back_low = true;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  uint32_t current_time;  // local variable for current time
  bool SW_high;

  current_time = millis();
  if (current_time > debounce_time + DEBOUNCE_INTERVAL) {
    SW_high = digitalRead(SW) == HIGH;
 //   Serial.println(current_state);
    if (SW_went_back_low && SW_high) {
      SW_went_back_low = false;
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
    } else if (!SW_went_back_low && !SW_high) {
      SW_went_back_low = true;
    }
    debounce_time = current_time;
  }
}

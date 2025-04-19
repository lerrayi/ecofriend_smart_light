// Creates a smart light that only turns on when movement is detected nearby.
// The light automatically turns off after a period of time specified by max_time (for demonstration, it is set to 10 seconds).
// Information about how long the light has been on is printed to an LCD.

#include <LiquidCrystal.h>

unsigned long start_time;
unsigned long current_time = 0;
unsigned long elapsed_time = 0;
int minutes;
int seconds;
unsigned long max_time = 10;

const int trigPin = 8;
const int echoPin = 7;

float duration, distance;

int on = 0;
int sensitivity = 200;
int led = 13;

// Tracks the pin numbers used for the LCD
const int rs = 12, 
          en = 11, 
          d4 = 5, 
          d5 = 4, 
          d6 = 3, 
          d7 = 2;

// Initializes LCD with pin numbers
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // LCD setup
  lcd.begin(16, 2);
  lcd.clear();
  Serial.begin(9600);
  
  // Input/output setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  // Takes ultrasonic input and determines distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2; 

  Serial.print("Distance: ");
  Serial.println(distance);

  lcd.setCursor(0,0);

  // Determines if motion is detected within set range
  if ((distance <= sensitivity) && (distance > 0)) {
    lcd.clear();
    Serial.println("Light on.");
    on = 1;
    lcd.print("Turning on...");
    digitalWrite(led, HIGH);
    delay(5000);
    lcd.clear();

    // Timer resets every time motion is detected
    start_time = millis();
  }

  current_time = millis();
  elapsed_time = (current_time - start_time) / 1000; // In seconds

  if (on == 1) {
    minutes = elapsed_time / 60;
    seconds = elapsed_time % 60;

    lcd.print("On for: ");

    // If statements add leading 0's to clock
    if (minutes < 10) {
      lcd.print("0");
    }
    lcd.print(minutes);
    lcd.print(":");
    if (seconds < 10) { 
      lcd.print("0");
    }
    lcd.print(seconds);
  } else {
    lcd.print("Light is off.");
  }

  // If more than max_time has passed, turn the light off
  if (elapsed_time >= max_time) {
      Serial.println("Light off.");
      digitalWrite(led, LOW);
      lcd.clear();
      on = 0;
  }

}

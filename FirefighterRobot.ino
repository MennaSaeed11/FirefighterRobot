#include <Arduino.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Servo Initialization 
Servo myservo;
// Servo Position
int pos = 0;
// IF fire exists or not
boolean fire = false;

// Motor Pins
#define IN1 17
#define IN2 5
#define IN3 18
#define IN4 19

// Flame Sensors
#define flameL 36  // Left
#define flameM 39  // Middle
#define flameR 34  // Right

// Buzzer Pin
#define buzzer 23

// Smoke Sensor
#define smoke 35

// Servo Pin
#define servoPin 32

// LCD Initialization
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C Address, 16 columns, 2 rows

// Function declarations
void _stop();
void forword();
void backword();
void right();
void left();
void _buzzer();
void put_off_fire();

void setup() {
    Serial.begin (115200);
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Fire Robot Ready");
    delay(2000);
    lcd.clear();

    // Motor pins setup
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    _stop();  // Start with motors off

    // Flame sensor pins setup
    pinMode(flameL, INPUT);
    pinMode(flameM, INPUT);
    pinMode(flameR, INPUT);

    // Buzzer pin setup
    pinMode(buzzer, OUTPUT);

    // Smoke sensor setup
    pinMode(smoke , INPUT);

    // Servo setup
    myservo.attach(servoPin);
    myservo.write(90);  // Start with servo at 90 degrees
}

// Function to simulate extinguishing fire
void put_off_fire() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Extinguishing...");
    
    _stop();  // Stop movement
    delay(250);
    
    // Sweep servo to simulate putting out fire
    for (pos = 50; pos <= 130; pos += 1) { 
        myservo.write(pos); 
        delay(10);  
    }
    for (pos = 130; pos >= 50; pos -= 1) { 
        myservo.write(pos); 
        delay(10);
    }

    // Reset servo to default
    myservo.write(90);
    fire = false;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fire Extinguished");
    delay(2000);
    lcd.clear();
}

// Function for car to move forward
void forword() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moving Forward");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, LOW);
}

// Function for car to move backward
void backword() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Moving Backward");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, HIGH);
}

// Function for car to move right
void right() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turning Right");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, HIGH); 
    digitalWrite(IN4, LOW);
}

// Function for car to move left
void left() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Turning Left");
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, LOW); 
    digitalWrite(IN4, LOW);
}

// Function to stop the car
void _stop() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Stopped");
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW); 
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}

// Function for buzzer alert
void _buzzer() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Fire Alert!");
    for (int i = 0; i < 3; i++) {  // Beep 3 times
        digitalWrite(buzzer, HIGH);
        delay(500);
        digitalWrite(buzzer, LOW);
        delay(500);
    }
}

void loop() {
    // Read smoke sensor value (if needed for additional conditions)
    int smokeval = analogRead(smoke);
    Serial.println(smokeval);

    // Reset servo to default position
    myservo.write(90);

    // If no fire is detected on all flame sensors
    if (digitalRead(flameL) == 1 && digitalRead(flameM) == 1 && digitalRead(flameR) == 1) {
        _stop();  // Stop the car
    } 
    // Fire detected straight ahead (middle sensor)
    else if (digitalRead(flameM) == 0) {
        forword();  // Move forward
        delay(100);  // Move for a short duration
        _buzzer();   // Sound buzzer
        _stop();     // Stop movement
        fire = true; // Fire detected, activate fire extinguishing
    } 
    // Fire detected to the left
    else if (digitalRead(flameL) == 0) {
        left();      // Move left
        delay(100);  // Adjust duration as needed
        _buzzer();   // Sound buzzer
        _stop();     // Stop movement
        fire = true; // Fire detected, activate fire extinguishing
    } 
    // Fire detected to the right
    else if (digitalRead(flameR) == 0) {
        right();     // Move right
        delay(100);  // Adjust duration as needed
        _buzzer();   // Sound buzzer
        _stop();     // Stop movement
        fire = true; // Fire detected, activate fire extinguishing
    }

    delay(300);  // Slow down the speed of the robot

    // If fire is detected, start extinguishing it
    while (fire == true) {
        put_off_fire();  // Call function to put off fire
    }
}

// HC-SR04 pinout to arduino Analog pins
#define PIN_TRIG A3
#define PIN_ECHO A2

#include <TimerOne.h>
#include <TM74HC595Display.h>

//---- TM1637 to arduino digital pins ----
byte SCLK = 4;
byte RCLK = 3;
byte DIO = 2;
//---- TM1637 to arduino digital pins ----

TM74HC595Display disp(SCLK, RCLK, DIO);

float meters;
long duration, prevTimer;
long check_interval = 1000; //Check every 1 second

void setup() {
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  Timer1.initialize(1500);
  Timer1.attachInterrupt(timerIsr);  // Init timer for every 1500 microseconds for display refresh
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - prevTimer <= check_interval) { //Do we need to check the module for distance
    return;
  }
  
  prevTimer = currentMillis;

  noInterrupts(); //We need to stop display refreshing while working with time critical measures

  //Make walve
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(PIN_TRIG, HIGH);

  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  duration = pulseIn(PIN_ECHO, HIGH); //Wait for the signal and check the delay

  interrupts(); //Resume display refreshing

  meters = (duration / 2) / 29.1 / 100; //Caclulate distance in meters
  disp.clear();
  disp.float_dot(meters, 2); //Display
}

void timerIsr() {
  disp.timerIsr(); //Just refresh the display
}

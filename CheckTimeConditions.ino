#include <DS3231.h>
#include <Encoder.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define ENCODER_DO_NOT_USE_INTERRUPTS

LiquidCrystal_I2C lcd(0x27, 16, 2);
DS3231  rtc(SDA, SCL);
Time current;
Encoder myEnc(5, 6);

int encodeButton = 7;
int relayPin = 10;
int notActivePin = 11;
int resetSwitch = 12;
int startHour;
int stopHour;
int encoderInput;
int counter;
int startTime;
int stopTime;
int tmpHour;
int button;
int resetButton;
final int startMin = 0;
final int stopTime = 5;

boolean startTimeSet = false;
boolean stopTimeSet = false;


void setup() {
  Serial.begin(9600);
  
  Wire.begin();
  
  rtc.begin();
  lcd.backlight();
  lcd.begin();
  
  pinMode(encodeButton, INPUT_PULLUP);
  pinMode(relayPin, OUTPUT);
  pinMode(notActivePin, OUTPUT);
  pinMode(resetButton, INPUT_PULLUP);

  digitalWrite(relayPin, HIGH);
  digitalWrite(notActivePin, HIGH); //Initalizing the relay to state and status LED's to "off"
  digitalWrite(encodeButton, HIGH);
  digitalWrite(resetButton, HIGH);

  startTimeSet = false;
  stopTimeSet = false;


}


void loop() {
  current = rtc.getTime();

  checkTimeHourConditions(current.hour, startTime, stopTime);
  checkTimeMinConditions(current.hour, startMin, stopMin);
  debounce();
  isValidInput(tmpHour);
  menuOptions();

}


void debounce() {

  encoderInput = myEnc.read();

  if (encoderInput % 4 == 0) {
    tmpHour = encoderInput / 4;
  }

}

void menuOptions (){

button = digitalRead(encodeButton);
resetButton = digitalRead(resetSwitch);

if (startTimeSet == false) {

  lcd.setCursor(0, 0);
  lcd.print("Start : ");
  if (tmpHour < 10) {
  lcd.print("0"); }
  lcd.print(tmpHour);
  lcd.print(" ");

  if (button == LOW) {
    startTime = tmpHour;
    lcd.clear();
    startTimeSet = true;
    myEnc.write(0);
    tmpHour = 0;
    delay(300);
  }

}

else if (stopTimeSet == false) {
 
  lcd.setCursor(0, 0);
  lcd.print("Stop : ");
  if (tmpHour < 10) {
  lcd.print("0"); }
  lcd.print(tmpHour);
  lcd.print(" ");

  if (button == LOW) {
    stopTime = tmpHour;
    lcd.clear();
    stopTimeSet = true;
    myEnc.write(0);
    tmpHour = 0;
    delay(300);
  }
}



if (startTimeSet == true && stopTimeSet == true) {
  lcd.setCursor(0, 0);
  lcd.print("Schedule: ");
  
  if (startTime < 10) {
  lcd.print("0"); }
  
  lcd.print(startTime);
  lcd.print(" > ");

  if (stopTime < 10) {
  lcd.print("0"); }
  
  lcd.print(stopTime);

}

if (resetButton == LOW) {

  startTimeSet = false;
  stopTimeSet = false;
  startTime = 0;
  stopTime = 0;
  lcd.clear();
}

}


void isValidInput(int tmpHour) {

  if (tmpHour > 23) {
    myEnc.write(0);
  }

  if (myEnc.read() < 0) {
    myEnc.write(23 * 4);
  }

}

void checkTimeHourConditions(int currentHour, int startTime, int stopTime) { //Compares current hour with user input start/stop hour

  if (currentHour >= startTime && currentHour < stopTime) {

    digitalWrite(relayPin, LOW);
    digitalWrite(notActivePin, HIGH);
  }


  else if (currentHour < startTime || currentHour >= stopTime) {

    digitalWrite(relayPin, HIGH);
    digitalWrite(notActivePin, LOW);
  }
}

void checkTimeMinConditions(int currentMin, int startTime, int stopTime) { //Compares current min with user input start/stop min

  if (currentMin >= startTime && currentMin < stopTime) {

    digitalWrite(relayPin, LOW);
    digitalWrite(notActivePin, HIGH);
  }


  else if (currentMin < startTime || currentMin >= stopTime) {

    digitalWrite(relayPin, HIGH);
    digitalWrite(notActivePin, LOW);
  }
}

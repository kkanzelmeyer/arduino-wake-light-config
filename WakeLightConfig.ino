#include <DS3232RTC.h>        //http://github.com/JChristensen/DS3232RTC
#include <Streaming.h>        //http://arduiniana.org/libraries/streaming/
#include <Time.h>             //http://playground.arduino.cc/Code/Time
#include <Wire.h>             //http://arduino.cc/en/Reference/Wire

#define ALARM_PIN 2

// alarm 1
#define AL1_HOUR 7
#define AL1_MIN 0

// alarm 2
#define AL2_HOUR 15
#define AL2_MIN 0

// time
#define TIME_HOUR 19
#define TIME_MIN 24
#define DAY 31
#define MONTH 3
#define YEAR 16

void setup() {

  // initialize
  Serial.begin(115200);
  setSyncProvider(RTC.get);
  Serial << F("RTC Sync");
  if (timeStatus() != timeSet) Serial << F(" FAIL!");
  Serial << endl;
  delay(200);
  
  
  // set time
  // (hour, minute, second, day, month, year)
  Serial.println("Setting time");
  setTime(TIME_HOUR, TIME_MIN, 0, DAY, MONTH, YEAR);
  RTC.set(now());
  delay(200);

  // set alarms
  // setAlarm(type, minutes, hours, null);
  Serial.println("Setting alarm 1");
  RTC.setAlarm(ALM1_MATCH_HOURS, AL1_MIN, AL1_HOUR, 0);
  delay(200);

  Serial.println("Setting alarm 2");
  RTC.setAlarm(ALM2_MATCH_HOURS, AL2_MIN, AL2_HOUR, 0);
  delay(200);

  Serial.println("Enabling alarm 1");
  RTC.alarmInterrupt(ALARM_1, false);
  delay(200);
  RTC.alarmInterrupt(ALARM_1, true);
  delay(200);

  Serial.println("Enabling alarm 2");
  RTC.alarmInterrupt(ALARM_2, false);
  delay(200);
  RTC.alarmInterrupt(ALARM_2, true);
  delay(200);

  // read time
  time_t myTime;
  myTime = RTC.get();
  printDateTime(myTime);


  // Prepare the interrupt pin
  pinMode(ALARM_PIN, INPUT_PULLUP);
}

void loop() {
//  Serial.println("Enabling ISR");
//  attachInterrupt(digitalPinToInterrupt(ALARM_PIN), IsrAlarmInterrupt, LOW);
//  delay(200);

  Serial.println("Waiting for alarm...");
  while(digitalRead(ALARM_PIN)==HIGH) {

    // user can prompt for time
    if (Serial.available() > 0) {
      Serial.read();
      time_t myTime;
      myTime = RTC.get();
      printDateTime(myTime);
      delay(1000);
    }
    
  }

  // Reset the alarm flag
  if(RTC.alarm(ALARM_1))
    Serial.println("Alarm 1 Activated. Enabling LED");
  if(RTC.alarm(ALARM_2))
    Serial.println("Alarm 2 Activated. Enabling LED");
  
  // Display time
  time_t myTime;
  myTime = RTC.get();
  printDateTime(myTime);
  delay(200);

  //enable built in LED
  digitalWrite(13, HIGH);
  delay(5000);
  digitalWrite(13, LOW);

}

//print date and time to Serial
void printDateTime(time_t t)
{
    printDate(t);
    Serial << ' ';
    printTime(t);
    Serial.println("");
}

//print time to Serial
void printTime(time_t t)
{
    printI00(hour(t), ':');
    printI00(minute(t), ':');
    printI00(second(t), ' ');
}

//print date to Serial
void printDate(time_t t)
{
    printI00(day(t), 0);
    Serial << monthShortStr(month(t)) << _DEC(year(t));
}

//Print an integer in "00" format (with leading zero),
//followed by a delimiter character to Serial.
//Input value assumed to be between 0 and 99.
void printI00(int val, char delim)
{
    if (val < 10) Serial << '0';
    Serial << _DEC(val);
    if (delim > 0) Serial << delim;
    return;
}

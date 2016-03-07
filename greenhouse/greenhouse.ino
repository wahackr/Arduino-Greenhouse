/**
 * I2C/IIC LCD Serial Adapter Module Example
 * Tutorial by http://mklec.com
 * 
 * Instructions at http://blog.mklec.com/how-to-use-iici2c-serial-interface-module-for-1602-lcd-display
 *
 * This uses the Liquid Crystal library from https://bitbucket.org/fmalpartida/new-liquidcrystal/downloads GNU General Public License, version 3 (GPL-3.0)
 * Pin Connections: 
 *      SCL = A5
 *      SDA = A4
 *      VCC = 5V
 *      GND = GND
 */

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS3231.h>
#include <DHT.h>;

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
#define ONE_WIRE_BUS 3
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C  lcd(0x27,2,1,0,4,5,6,7); // 0x27 is the I2C bus address for an unmodified module
DS3231  rtc(SDA, SCL); // Init the DS3231 using the hardware interface

float hum;  //Stores humidity value
float temp_inside; //Stores temperature value
float temp_outside; //Stores temperature value
int temp = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.setBacklightPin(3, POSITIVE);
  lcd.setBacklight(HIGH); // NOTE: You can turn the backlight off by setting it to LOW instead of HIGH
  lcd.begin(16, 2); //16 columns, 2 lines
  lcd.clear();  
  rtc.begin();
  dht.begin();
  //rtc.setTime(19,42,00);
  //rtc.setDate(5,3,2016);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Send Day-of-Week
  Serial.print(rtc.getDOWStr());
  Serial.print(" ");
  
  // Send date
  Serial.print(rtc.getDateStr());
  Serial.print(" -- ");

  // Send time
  Serial.println(rtc.getTimeStr());

  sensors.requestTemperatures();
  temp_outside = sensors.getTempCByIndex(0);

  hum = dht.readHumidity();
  temp_inside= dht.readTemperature();
  
  Serial.print("Outside Temperature: ");
  Serial.println(temp_outside);
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println("%"); 
  Serial.print("Inside Temperature: ");
  Serial.print(temp_inside);
  Serial.println(" Celsius");

  lcd.setCursor(0,0);
  lcd.print(rtc.getDateStr());
  lcd.print(" ");
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0,1);
  if (temp == 0) {
    lcd.print("In: ");
    lcd.print((int)temp_inside);
    temp = 1;
  } else {
    lcd.print("Out: ");
    lcd.print((int)temp_inside);
    temp = 0;
  }
  lcd.print(" Hum: ");
  lcd.print(hum, 0);
  lcd.print("%");
  delay(5000);
}

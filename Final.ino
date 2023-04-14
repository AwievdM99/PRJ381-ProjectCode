#include <DHTesp.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define DHT11_PIN 2

DHTesp dht;
LiquidCrystal_I2C lcd(0x27, 20, 4);

const int sensor_pin = A0;
//Network Name
const char *ssid     = "";
//Network Password
const char *password = "";
const long utcOffsetInSeconds = 7200;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

uint8_t Light = D5;

void setup() {
  Serial.begin(9600);
  Wire.begin(D2, D1);  
  lcd.begin(20,4);                          
  lcd.backlight();      
  lcd.home();
  String thisBoard= ARDUINO_BOARD;
  Serial.println(thisBoard);
  dht.setup(2, DHTesp::DHT11);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  timeClient.begin();

  pinMode(Light, OUTPUT);
digitalWrite(Light, HIGH);  
}
void loop() {         
  delay(dht.getMinimumSamplingPeriod());
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  float moisture_percentage;
  moisture_percentage = ( 100.00 - ( (analogRead(sensor_pin)/1023.00) * 100.00 ) );  
  lcd.clear();
  lcd.setCursor(0, 0);  
  lcd.print("Temperature");
  lcd.setCursor(12, 0);  
  lcd.print(temperature);
  lcd.setCursor(16, 0);  
  lcd.print("C");
  lcd.setCursor(0, 1);  
  lcd.print("Humidity");
  lcd.setCursor(9, 1); 
  lcd.print(humidity);
  lcd.setCursor(14, 1);
  lcd.print("%");
  lcd.setCursor(0, 2);  
  lcd.print("Soil Moisture");
  lcd.setCursor(14, 2); 
  lcd.print(moisture_percentage);
  lcd.setCursor(19, 2);
  lcd.print("%"); 
  timeClient.update();
  lcd.setCursor(0, 3);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.setCursor(9, 3);
  lcd.print(timeClient.getHours());
  lcd.setCursor(11, 3);
  lcd.print(":");
  lcd.setCursor(12, 3);
  lcd.print(timeClient.getMinutes());

  if (timeClient.getHours() >= 6 && timeClient.getHours() < 18) {
    digitalWrite(Light, LOW);
  }
  else {
  digitalWrite(Light, HIGH);
  }

  delay(10000);
}
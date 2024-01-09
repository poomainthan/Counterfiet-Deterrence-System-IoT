#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int UVOUT = A0; //Output from the sensor
int REF_3V3 = A1; //3.3V power on the Arduino board
const int buttonPin = 2;  // the number of the pushbutton pin
//int buttonState = LOW;
int prebuttonState = LOW;
void setup()
{
  Serial.begin(9600);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  pinMode(UVOUT, INPUT);
  pinMode(REF_3V3, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.println("..........CODE BY KIT FREE_THINKERS.............");
  
}
void loop()
{
  int buttonState = digitalRead(buttonPin);
  int uvLevel = averageAnalogRead(UVOUT);
  int refLevel = averageAnalogRead(REF_3V3);
  //Use the 3.3V power pin as a reference to get a very accurate output value from sensor
  float outputVoltage = 3.3 / refLevel * uvLevel;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.8, 0.0, 15.0); //Convert the voltage to a UV intensity level
  Serial.print("output: ");
  Serial.print(refLevel);
  Serial.print("-----ML8511 output: ");
  Serial.print(uvLevel);
  Serial.print(" / ML8511 voltage: ");
  Serial.print(outputVoltage);
  Serial.print(" / UV Intensity (mW/cm^2): ");
  Serial.print(uvIntensity);
  lcd.clear();
  if(buttonState == LOW)
  {
    lcd.setCursor(0,0);
    lcd.print("PLACE DOCUMENT");
    lcd.setCursor(0,1);
    lcd.print("AND PRESS BUTTON");
  }
  else if(buttonState == HIGH && uvIntensity<=-0.07)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Currency note is..");
    lcd.setCursor(3, 1 );
    lcd.print("DETECTED..!");
    Serial.print(".....THERE MIGHT BE A CURRENCY......");
  }
  else if(buttonState== HIGH && uvIntensity>=-0.06)
  {
   lcd.clear();
   lcd.print("Printable paper");
   Serial.print("..........Printable paper..........");
   
  }
  
  buttonState = prebuttonState;
  Serial.println();
  delay(100);
  exit(0);
  
}

int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
 
  return(runningValue);
}
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


#include <LiquidCrystal.h>
LiquidCrystal lcd(4,6,10,11,12,13); 

const int toneButton = 3;
const int buttonUp = 7;
const int buttonDown = 8;
const int increment = 1;
int frequencyTarget=100;
const int maxFrequency=399;
const int minFrequency=2;
const int DO = 2; //Pin for Digital Output - DO
const int DA = A0; // Pin for Analog Output - AO
int sensorvalue = 0;


void setup()
{
  lcd.begin (16,2);
  pinMode(buttonUp,INPUT);
  pinMode(buttonDown,INPUT);
  lcd.setCursor (0,0);
  lcd.print ("Frequency:");
  lcd.setCursor (10,0);
  lcd.print (frequencyTarget);
  

}

void loop() {
  sensorvalue = analogRead(DA);  //Read the analog value
  lcd.setCursor(0,1);
  lcd.print("Analog:");
  lcd.print(sensorvalue);
  delay(150);
  
lcd.setCursor (10,0);
if(digitalRead (buttonUp) == HIGH && digitalRead(buttonDown) == LOW )
  {
    //tone(9, frequencyTarget);
    if(frequencyTarget <= maxFrequency)
    {
    frequencyUp();
    }
  }
 if(digitalRead (buttonDown) == HIGH && digitalRead(buttonUp) == LOW )
  {
    //tone(9, frequencyTarget);
    if(frequencyTarget >= minFrequency)
    {
    frequencyDown();
    }
  }
 if(frequencyTarget < 100)
  {
    lcd.setCursor(12,0);
    lcd.print(" ");
  }
 if(digitalRead(toneButton) == HIGH)
  {
    tone(9, frequencyTarget);
  }
 if(digitalRead(toneButton) == LOW)
  {
    noTone(9);
  }
}
void frequencyUp()
{
    frequencyTarget = frequencyTarget + increment;
    lcd.print(frequencyTarget);  
    delay(150);
}

void frequencyDown()
{
    frequencyTarget = frequencyTarget - increment;
    lcd.print(frequencyTarget);
    delay(150);
}



 


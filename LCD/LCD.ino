#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(4,6,10,11,12,13); 

unsigned int frequencyTarget=200;
unsigned int frequency;

const int maxFrequency=300;
const int minFrequency=32;
const int increment = 1;
const int toneButton = 3;
const int buttonUp = 7;
const int buttonDown = 8;




void setup()
{
  
  Wire.begin(8);
  lcd.begin (16,2);
  
  lcd.setCursor (0,0);
  lcd.print ("Selector: ");
  lcd.setCursor(0,1);
  lcd.print("Frequency: ");
  
  lcd.setCursor (10,0);
  lcd.print (frequencyTarget);
}

void loop() 
{

  Wire.onReceive(receiveEvent);
  PushButton();
  ScreenAdjustment();
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

void receiveEvent(int bytes)
{
lcd.setCursor(11,1);
int x = Wire.read();
lcd.print(x);
}


void PushButton()
{
  
  lcd.setCursor (10,0);
  if(digitalRead (buttonUp) == HIGH && digitalRead(buttonDown) == LOW ){
    if(frequencyTarget < maxFrequency){
      frequencyUp();
    }
  }
  if(digitalRead (buttonDown) == HIGH && digitalRead(buttonUp) == LOW ){
    if(frequencyTarget > minFrequency){
      frequencyDown();
    }
  }
  if(digitalRead(toneButton) == HIGH){
    tone(9, frequencyTarget);
  }
  if(digitalRead(toneButton) == LOW){
    noTone(9);
  }
}

void ScreenAdjustment()
{

  //selector adjustment
  if(frequencyTarget < 10){
    lcd.setCursor(11,0);
    lcd.print(" ");
  }
  if(frequencyTarget < 100){
    lcd.setCursor(12,0);
    lcd.print(" ");
  }  

  //frequency adjustment
  if(frequency < 10){
    lcd.setCursor(12,1);
    lcd.print(" ");
  }
  if(frequency < 100){
    lcd.setCursor(13,1);
    lcd.print(" ");  
  }
  if(frequency < 1000){
    lcd.setCursor(14,1);
    lcd.print(" ");
  }
}


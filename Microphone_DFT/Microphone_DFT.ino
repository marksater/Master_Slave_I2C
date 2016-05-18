#include <Wire.h>

const int button = 3;
const int mic = A0;
const float pi = 3.141592654;
const float N = 600.0; //Number of samples
float T; // k is divided by delta times number of samples or divided by t (Time)
float f[600];  //Signal array
float xSum, ySum, delta;
signed long startTime, endTime, elTime;
float Fx[300], Fy[300]; // Real and imaginary components of transform
float Mag[300], Freg[300];
unsigned int k,n,r,i ;
boolean dftProcess = false;
boolean recording = false;

void setup() 
{
  
  Wire.begin();
  Serial.begin(9600);
}

void loop() 
{
  int buttonPush = digitalRead(button);
  
  if(buttonPush == HIGH && dftProcess == false && recording == false){
    Record();
  }
  Serial.println(buttonPush);
  if(dftProcess == true && recording == false){
    byte Transmission=0;  
    for(k=0; k <= N/2-1; k++){
      xSum=0; ySum =0;
      for(n=0;n<=N-1;n++){
        float arg = 2*pi*k*n/N;
        xSum = xSum + f[n]*cos(arg);
        ySum = ySum + f[n]*sin(arg);
      }
      Fx[k]=(1.0/N)*xSum;
      Fy[k]=(1.0/N)*(-ySum);
      Mag[k]=sqrt( pow(Fx[k],2)+pow(Fy[k],2) );
      Freg[k]=float(k)/T;// k/(T)or you can use k/(N*delta)
    }
    Transmission = GetMaxFrequency();
    Wire.beginTransmission(8);
    Wire.write(Transmission);
    Wire.endTransmission();
    dftProcess = false;
    Serial.print("end of dft");Serial.print("byte: ");Serial.println(sizeof(Transmission));
  }
}


float GetMaxFrequency()
{
  float maxFreq = 0;
  
  for (int i = 0; i <= N-1; i++){
    maxFreq = max(Mag[i],maxFreq);
  }
  
  for (int i = 0; i <=N-1;i++){
    if (Mag[i] == maxFreq){
      return Freg[i];
    }
  }
}

void Record()
{
  
  recording = true;
  Microphone();
  recording = false;
  dftProcess = true;
}

void Microphone()
{
  float sum = 0;
  float Average;
  
  startTime = millis();
  for(r=0;r<=N-1;r++){
    f[r]=analogRead(mic)*(5.0/1023.0);
    sum=sum+f[r];
    delay(2);
  }
  endTime = millis();
  elTime = endTime - startTime;
  delta = (elTime / N)/1000.0;
  T = N*delta; // changed to N instead of N-1
  Average=sum/N;
  for(r=0;r<=N-1;r++){
    f[r]=500*(f[r]-Average);
    Serial.println(f[r]);
  }
  Serial.println("================================================="); 
}



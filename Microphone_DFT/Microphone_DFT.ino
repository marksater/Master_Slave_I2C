#include <Wire.h>

const int button = 3;
const int mic = A0;
const int N = 600; //Number of samples
const float pi = 3.141592654;
float T; // k is divided by delta times number of samples or divided by t (Time)
float f[N];  //Signal array
float xSum, ySum, delta;
signed long startTime, endTime, elTime;
float Fx[N/2], Fy[N/2]; // Real and imaginary components of transform
float Mag[N/2], Freq[N/2];
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
  //record the signal
  int buttonPush = digitalRead(button);
  if(buttonPush == HIGH && dftProcess == false && recording == false){
    Record();
  }

  //run the transform
  if(dftProcess == true && recording == false){
    byte Transmission=0;
    DFT();   
    Wire.beginTransmission(8);// begin transmission to uno
    Transmission = GetMaxFrequency();
    Wire.write(Transmission);
    Wire.endTransmission();
    dftProcess = false;
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
      return Freq[i];
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
  delta = (elTime / float(N) )/1000.0;
  T = N*delta; // changed to N instead of N-1
  Average = sum / float(N);
  for(r=0;r<=N-1;r++){
    f[r]=500*(f[r]-Average);
    Serial.println(f[r]);
  }
}

void DFT()
{
  
  for(k = 0; k <= N/2-1; k++){
    xSum = 0; ySum = 0;
    for(n = 0; n <= N-1; n++){
      float arg = 2*pi*k*n/float(N);
      xSum = xSum+f[n]*cos(arg);
      ySum = ySum+f[n]*sin(arg);
    }
    Fx[k] = (1.0/float(N))*xSum;
    Fy[k] = (1.0/float(N))*(-ySum);
    Mag[k] = sqrt( pow(Fx[k],2)+pow(Fy[k],2) );
    Freq[k] = float(k)/T;// k/(T)or you can use k/(N*delta)
  } 
}


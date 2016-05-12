#include <Wire.h>
//DFT!!!!! 
// time to read microphone pin is 1.12 milsec
// delay + 1.12 milsec = delta  in mill
const int mic = A0;
const float pi = 3.141592654;
const float N = 600.0; //Number of samples
float T; // k is divided by delta times number of samples or divided by t (Time)
float f[600];  //Signal array
float xSum, ySum, frequencyMax, delta;
signed long startTime, endTime, elTime;
float Fx[300], Fy[300]; // Real and imaginary components of transform
float Mag[300], Freg[300];
unsigned int k,n,r,i ;


void setup() {
Wire.begin();
Serial.begin(9600);
Serial.println("==============================================");
Serial.print("index");Serial.print("\t");Serial.print("time"); Serial.print("\t");Serial.print("sin/cos wave");Serial.print("\t");Serial.println("delta");
Serial.println("==============================================");

float sum = 0;
float Average;
startTime = millis();
  for(r=0;r<=N-1;r++){
    //f[r]=50*cos(2*pi*400*r*delta);
    f[r]=analogRead(mic)*(5.0/1023.0);
    sum=sum+f[r];
    delay(2);
//Serial.print(r);Serial.print("\t");Serial.print(r*delta); Serial.print("\t"); Serial.print(f[r]);Serial.print("\t");Serial.print("\t");Serial.println(delta*1000);
}
endTime = millis();
elTime = endTime - startTime;
Serial.println(elTime);Serial.println(N);
delta = (elTime / N)/1000.0;
T = (N-1)* delta;
Average=sum/N;
Serial.print(T);Serial.print(" ");Serial.print("delta ");Serial.print(delta,6);Serial.print(" ");Serial.print("average: ");Serial.println(Average);
 for(r=0;r<=N-1;r++){
  f[r]=500*(f[r]-Average);
Serial.println(f[r]);

  }
Serial.println(" ");
Serial.println("==============================================");
}

void loop() {
byte Tran=0;  
Serial.print("Index");Serial.print("\t");Serial.print("Real");Serial.print("\t");Serial.print("Image");Serial.print("\t");Serial.print("Mag");Serial.print("\t");Serial.println("Freg");
Serial.println("==============================================");   
  for(k=0; k <= N/2-1; k++){
    xSum=0; ySum =0;
    for(n=0;n<=N-1;n++){
      float x,y,arg;
      arg = 2*pi*k*n/N;
      x = f[n]*cos(arg);
      y = f[n]*sin(arg);
      xSum = xSum + x; //xSum = xSum + f[n]*cos(arg);
      ySum = ySum + y; //ySum = ySum + f[n]*sin(arg);
    }
    Fx[k]=(1.0/N)*xSum;
    Fy[k]=(1.0/N)*(-ySum);
    Mag[k]=sqrt( pow(Fx[k],2)+pow(Fy[k],2) );
    Freg[k]=float(k)/T;// k/(T)or you can use k/(N*delta)

                                                                                                                //notice mag is multiplied by two
Serial.print(k);Serial.print("\t");Serial.print(Fx[k]);Serial.print("\t");Serial.print(Fy[k]);Serial.print("\t"); Serial.print(2*Mag[k]);Serial.print("\t"); Serial.println(Freg[k]);

  }
    float maxFrequency = GetMaxFrequency();
    Serial.print("Max Frequency: ");
    Serial.print(maxFrequency);
    Serial.println(" Hz");
    Serial.print("==============================================");
    Serial.println(" ");

    Tran = GetMaxFrequency();
    Wire.beginTransmission(8);
    Wire.write(Tran);
    Wire.endTransmission();
    
    while(true)  
   {  }
}


float GetMaxFrequency(){
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






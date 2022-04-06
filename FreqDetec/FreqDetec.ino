

#include "arduinoFFT.h"
#include <Servo.h>
#define SAMPLES 128              
#define SAMPLING_FREQUENCY 4200
 
arduinoFFT FFT = arduinoFFT();
Servo waist,leftArm,rightArm;
int waistPos, leftPos, rightPos;
unsigned int samplingPeriod;
unsigned long microSeconds;
const int buttonPin = 2;
double vReal[SAMPLES]; 
double vImag[SAMPLES]; 
char notes[50];
int notetime[50];
int numNotes = 0;
int deltatime = 0;
unsigned long recent = 0;
float breaktime = 700;
void playNote(char Note){

  switch(Note){
    case 'C':
      waist.write(45);
      delay(50);
      lowerLeft();
      delay(150);
      break;
    case 'D':
      waist.write(30);
      delay(50);
      lowerLeft();
      delay(150);
      break;
    case 'E':
      waist.write(22);
      delay(50);
      lowerLeft();
      delay(150);
      break;
    case 'F':
      waist.write(10);
      delay(50);
      lowerLeft();
      delay(150);
      break;
    case 'G':
      waist.write(35);
      delay(50);
      lowerRight();
      delay(150);
      break;
    case 'A':
      waist.write(22);
      delay(50);
      lowerRight();
      delay(150);
      break;
    case 'B':
      waist.write(10);
      delay(50);
      lowerRight();
      delay(150);
      break;
    case 'H':
      waist.write(0);
      delay(50);
      lowerRight();
      delay(150);
      break;
    default:
      break;
  }
  
}
void setup() 
{
    pinMode(A1,OUTPUT);
    pinMode(A2,OUTPUT);
    pinMode(buttonPin, INPUT);
    digitalWrite(A1,HIGH);
    digitalWrite(A2,LOW);
    Serial.begin(9600); 
    samplingPeriod = round(1000000*(1.0/SAMPLING_FREQUENCY));
  
    waistPos = 0; 
    leftPos = leftArm.read(); 
    rightPos = rightArm.read();
    waist.attach(9);
    leftArm.attach(3);
    rightArm.attach(6);

   leftArm.write(0);
    rightArm.write(180);
    delay(5000);
    waist.write(30);
}
 
void loop() 
{  
  int buttonState = digitalRead(buttonPin);
  if(buttonState==HIGH){
    delay(2000);
    Serial.println("Recording");
    while(numNotes <50 && buttonState == HIGH){
       
    for(int i=0; i<SAMPLES; i++)
    {
        microSeconds = micros();  
     
        vReal[i] = analogRead(0);
        vImag[i] = 0; 

        while(micros() < (microSeconds + samplingPeriod));
 
    }
 
    /*Perform FFT on samples*/
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

    
    
    
    double peak = FFT.MajorPeak(vReal, SAMPLES, SAMPLING_FREQUENCY);
    
    
    if(peak > 1090 && peak < 2070){
      deltatime = millis() - recent;
      
      if(deltatime > breaktime){
        float temp = peak*(SAMPLES/SAMPLING_FREQUENCY);
        if(1090 < peak && peak < 1108){
          Serial.println("C");
          Serial.println(vReal[(int)floor(temp)]);
          notes[numNotes] = 'C';
          notetime[numNotes] = deltatime;
          numNotes++;
          recent = millis();
          Serial.println(deltatime);
          //delay(500);
        }
        else if(peak > 1210 && peak < 1224){
          Serial.println("D");
          Serial.println(vReal[(int)floor(temp)]);
          notes[numNotes] = 'D';
          notetime[numNotes] = deltatime;
          numNotes++;
          //delay(250);
          recent = millis();
          Serial.println(deltatime);
        }
        else if( 1340 < peak && peak < 1370){
          Serial.println("E");
          Serial.println(vReal[(int)floor(temp)]);
          notes[numNotes] = 'E';
          notetime[numNotes] = deltatime;
          numNotes++;
          //delay(250);
          recent = millis();
          Serial.println(deltatime);
        }
        else if(1440 < peak && peak < 1460){
          if(deltatime>breaktime*1.2){
            Serial.println("F");
            Serial.println(vReal[(int)floor(temp)]);
            notes[numNotes] = 'F';
            notetime[numNotes] = deltatime;
            numNotes++;
            //delay(250);
            recent = millis();
            Serial.println(deltatime);
          }
        }
        else if(1620 < peak && peak < 1630){
          Serial.println("G");
          Serial.println(vReal[(int)floor(temp)]);
          notes[numNotes] = 'G';
          notetime[numNotes] = deltatime;
          numNotes++;
          //delay(250);
          recent = millis();
          Serial.println(deltatime);
        }
        else if(1820 < peak && peak < 1840){
          Serial.println("A");
          Serial.println(vReal[(int)floor(temp)]);
          notes[numNotes] = 'A';
          notetime[numNotes] = deltatime;
          numNotes++;
          //delay(250);
          recent = millis();
          Serial.println(deltatime);
        }
        else if(2020 < peak && peak < 2045){
          Serial.println("B");
          Serial.println(vReal[(int)floor(temp)]);
          notes[numNotes] = 'B';
          notetime[numNotes] = deltatime;
          numNotes++;
          //delay(250);
          recent = millis();
          Serial.println(deltatime);
        }
        else if(2050 < peak && peak < 2070){
          Serial.println("C");
          Serial.println(vReal[(int)floor(temp)]);
          notes[numNotes] = 'H';
          notetime[numNotes] = deltatime;
          numNotes++;
          //delay(250);
          recent = millis();
          Serial.println(deltatime);
         }
        else;
      }
      
    }
    
    buttonState = digitalRead(buttonPin);
    }
  Serial.println("Playing Back");
  for(int i =0;i<numNotes;i++){
    Serial.println(notes[i]);
    if(i!= 0)delay(notetime[i]-breaktime);
    playNote(notes[i]);
  }
  memset(notes,0,sizeof(notes));
  memset(notetime,0,sizeof(notetime));
  numNotes = 0;
  recent = 0;
  Serial.println("End Play Back");
   
  }
   

    
    
}



void lowerLeft(){
  for (leftPos = leftArm.read(); leftPos <= 48; leftPos += 6) { 
    // in steps of 1 degree
    leftArm.write(leftPos);             
    delay(35);                       
  }
  for (leftPos = leftArm.read(); leftPos > 0; leftPos -= 6) { 
    leftArm.write(leftPos);             
    delay(35);                       
  }
}
void lowerRight(){
  for (rightPos = rightArm.read(); rightPos >= 135; rightPos -= 6) { 
    // in steps of 1 degree
    rightArm.write(rightPos);             
    delay(35);                       
  }
  for (rightPos = rightArm.read(); rightPos < 180; rightPos += 6) { 
    rightArm.write(rightPos);             
    delay(35);                       
  }
}

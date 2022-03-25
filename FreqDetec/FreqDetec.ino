

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
char notes[180];
int numNotes = 0;
void playNote(char Note){

  switch(Note){
    case 'C':
      waist.write(0);
      delay(50);
      lowerLeft();
      delay(150);
      break;
    case 'D':
      waist.write(15);
      delay(50);
      lowerLeft();
      delay(150);
      break;
    case 'E':
      waist.write(30);
      delay(50);
      lowerLeft();
      delay(150);
      break;
    case 'F':
      waist.write(45);
      delay(50);
      lowerLeft();
      delay(150);
      break;
    case 'G':
      waist.write(0);
      delay(50);
      lowerRight();
      delay(150);
      break;
    case 'A':
      waist.write(15);
      delay(50);
      lowerRight();
      delay(150);
      break;
    case 'B':
      waist.write(30);
      delay(50);
      lowerRight();
      delay(150);
      break;
    case 'H':
      waist.write(45);
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

    waist.write(0);
    leftArm.write(0);
    rightArm.write(0);
    delay(5000);
    //waist.write(50);
}
 
void loop() 
{  
  int buttonState = digitalRead(buttonPin);
  if(buttonState==HIGH){
    delay(3000);
    buttonState = digitalRead(buttonPin);
    Serial.println("Recording");
    //buttonState = LOW;
    while(numNotes <180 && buttonState == HIGH){
        /*Sample SAMPLES times*/
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
      //Serial.print("");
      //Serial.println(deltatime);
      //if(deltatime < 4);
      if(1090 < peak && peak < 1108){
        Serial.println("C");
        notes[numNotes] = 'C';
        numNotes++;
        //timer = newtimer;
        delay(500);
      }
      else if(peak > 1210 && peak < 1224){
        Serial.println("D");
        notes[numNotes] = 'D';
        numNotes++;
        delay(250);
      }
      else if( 1340 < peak && peak < 1370){
        Serial.println("E");
        notes[numNotes] = 'E';
        numNotes++;
        delay(250);
      }
      else if(1440 < peak && peak < 1460){
        Serial.println("F");
        notes[numNotes] = 'F';
        numNotes++;
        delay(250);
      }
      else if(1620 < peak && peak < 1630){
        Serial.println("G");
        notes[numNotes] = 'G';
        numNotes++;
        delay(250);
      }
      else if(1820 < peak && peak < 1840){
        Serial.println("A");
        notes[numNotes] = 'A';
        numNotes++;
        delay(250);
      }
      else if(2020 < peak && peak < 2045){
        Serial.println("B");
        notes[numNotes] = 'B';
        numNotes++;
        delay(250);
      }
      else if(2049 < peak && peak < 2070){
        Serial.println("C");
        notes[numNotes] = 'H';
        numNotes++;
        delay(250);
       }
      else;
    }
    //delay(500);
    //buttonState = digitalRead(buttonPin);
    //if(numNotes > 5)buttonState = HIGH;
    }
  Serial.println("Playing Back");
  for(int i =0;i<180;i++){
    Serial.println(notes[i]);
    playNote(notes[i]);
  }
  Serial.println("End Play Back");
   
  }
   

    
    
}



void lowerLeft(){
  for (leftPos = leftArm.read(); leftPos <= 30; leftPos += 1) { 
    // in steps of 1 degree
    leftArm.write(leftPos);             
    delay(15);                       
  }
  for (leftPos = leftArm.read(); leftPos > 0; leftPos -= 1) { 
    leftArm.write(leftPos);             
    delay(15);                       
  }
}
void lowerRight(){
  for (rightPos = rightArm.read(); rightPos <= 30; rightPos += 1) { 
    // in steps of 1 degree
    rightArm.write(rightPos);             
    delay(15);                       
  }
  for (rightPos = rightArm.read(); rightPos > 0; rightPos -= 1) { 
    rightArm.write(rightPos);             
    delay(15);                       
  }
}

/*Changes to Iakov's Pretty Code:
   change 'A' to a space to allow greater separation between readings
   change "New Baseline" to "_________" for style
   modify delays throughout to maximize accuracy at slight cost of performance
   attempt to add in smoothing function of five values to improve linearity
   */

#include <SPI.h>

const int MUXCLOCK = 1;

const int RESET_NOT = PB_5;     //low to disable output, high to enable
const int DIN = PB_7;                    //select line to MUX
const int DOUT = PB_6;                //MUX feedback to TM4
const int CLOCK = PB_4;              //manually changed clock for MUX
const int CS_NOT = PA_3;            //low to allow changes to outputs, high to lock
const int BaseSize = 50;              //number of samples averaged for the baseline
const int thresh = 100 ;                //threshhold for delta
const int deltaSize = 15;                //size of delta array
const int pinNumber = 8;
const int SMOOTHINGSIZE = 2;   //size of array to hold values for smoothing
const int DELAY = 50;
const int absThreshold = 200;

int smoothingIndex = 0;

int smoothArray[pinNumber][SMOOTHINGSIZE];

int currentOutput = 0;                  //the current gate to be set as the output

int switchState[8];                       //Array determing pin to be read

int BaseArray[8];                           

int reading =0;                              //current reading from analog pins
int oldArray[8];                             //array of first reading on each pin
int newArray[8];                           //array of second reading on each pin
int counter = -1;
int a = -1;                                      //intex for the delta arrays

int delta[pinNumber][deltaSize];                                //arrays of delta values for each pin


boolean deltaSatisfactory = true;  //if this boolean is true, a new baseline will be taken


void setup()
{
  
  pinMode(RESET_NOT, OUTPUT);
  pinMode(DIN, OUTPUT);
  pinMode(DOUT, INPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(CS_NOT, OUTPUT);
  
  //Array indeces are reversed
 
  Serial.begin(9600);
  
  enableOutput();
  baseline();
}

void loop()
{
  Serial.println("A");                            //printed in order for matlab to get correct timing on readings
  for (int j = 0; j<8; j++)
  {
     counter++;
     currentOutput = j;
     setOutput(currentOutput);
     selectOutput();
     delayMicroseconds(750);
     reading = abs(BaseArray[j]-abs(analogRead(A1) - analogRead(A0)));

     if (counter < 8)                              //first 8 readings go into oldArray
       oldArray[j] = reading;
     else if (counter < 16)                   //seconds 8 readings go to newArray
       newArray[j] = reading;
     int actualVal = smooth(reading);
     Serial.println(actualVal, DEC);      //prints last read analog pin
     if (counter ==15)                  //on the sixteenth reading deltaCheck is called which if satisfactory resets the baseline
     {
       counter = -1;
       a++;
       //Serial.println("checking delta");
       deltaCheck();
     }
     delay(1);
  }
  
  delay(DELAY);
}

void setOutput(int input)     //setOutput creates the correct array to be sent to MUX in order to open corresponding pin 
{
  int n = 7-input;                     //currentOutput is the input for this function, and n is calculated since the order of pins on MUX is reversed
  for(int i=0; i<7; i++)
    switchState[i] = LOW;      //all pins are set to low, then the appropriate pin is set to high
  switchState[n] = HIGH;
}

void selectOutput()             //selectOutput sets CS_NOT to low in order to read the output, then opens and closes DIN and CLOCK to read pins
{
  digitalWrite(CS_NOT, LOW);
  
  delayMicroseconds(1);
  
  for(int i = 0; i < 8; i++)
  {
    digitalWrite(DIN, switchState[i]);
    delayMicroseconds(MUXCLOCK);
    digitalWrite(CLOCK, HIGH);
    delayMicroseconds(MUXCLOCK);
    digitalWrite(CLOCK, LOW);
  } 
    
  digitalWrite(CS_NOT, HIGH);
  
  delayMicroseconds(1);
}

void enableOutput()                                 //enables output
{
    digitalWrite(RESET_NOT, HIGH);
}

void disableOutput()                               //disables output
{
    digitalWrite(RESET_NOT, LOW);
}

void baseline()                                       //baseline function calculates new baseline when called
{

    for(int i=0; i<8; i++){
      int sum = 0;
      setOutput(i);
      selectOutput();
      delayMicroseconds(50);
      
      for(int k = 0; k < BaseSize; k++){
        sum+= abs(analogRead(A0) - analogRead(A1));
        delayMicroseconds(10);
      }
      BaseArray[i] = sum/BaseSize;
    }
   a = -1;
   
   for(int i = 0; i<pinNumber; i++){
     for(int j = 0; j<SMOOTHINGSIZE; j++)
       smoothArray[i][j] =  0;
   }
   
   smoothingIndex = 0;
  }
  
void deltaCheck()                                                             //deltaCheck checks the delta for several values on each pin and if the values are stable the baseline() func is called
  {
    boolean valSatisfactory = true;
    for ( int s = 0; s<pinNumber; s++)
   { 
    delta[s][a] = abs(oldArray[0] - newArray[0]);
   }
     
     if (a == deltaSize-1)
     {
       for(int s=0; s<pinNumber; s++)
       {
           for (int r=0; r<deltaSize; r++)
           {
               if (delta[s][r] < thresh)
                 deltaSatisfactory = true;
               else 
               {
                 deltaSatisfactory = false;
                 a = -1;
                 return;
               }
               delayMicroseconds(2);
           }
       }
         
       for (int s=0; s<8; s++)
       {
             if(newArray[s]>absThreshold)
             {
             valSatisfactory = false;
             a = -1;
             return;
             }
       }
     
          if (deltaSatisfactory && valSatisfactory)
          {
             baseline();
             //Serial.println("_______");
          }
           
     }

}

int smooth(int newValue)
{
  smoothArray[currentOutput][smoothingIndex] = newValue;
  smoothingIndex = (smoothingIndex+1)%SMOOTHINGSIZE;
  int sum = 0;
  
  for(int i = 0; i<SMOOTHINGSIZE; i++)
    sum+=smoothArray[currentOutput][i];
  
  return sum/SMOOTHINGSIZE;
}

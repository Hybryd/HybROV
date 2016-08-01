/*
Arduino code for the Hybryd ROV.
Author: Hybryd
Date: 01/08/2016

Feel free to reuse/share/improve this code.


Parts:
  DC Motor Vertical axis Right (MotorVR)
  DC Motor Vertical axis Left (MotorVL)
  DC Motor Horizontal plane Right (MotorHR)
  DC Motor Horizontal plane Left (MotorHL)
  Light Right (LightR)
  Light Left (LightL)
  
Inputs:
  JoystickA X axis     : analogic
  JoystickA Y axis     : analogic
  JoystickA Z axis     : analogic (usually digital)
  JoystickB Y axis     : analogic 

Outputs:
  Light Right          : digital
  Light Left           : digital
  MotorHR in1          : PWM
  MotorHR in2          : PWM
  MotorHR enable       : PWM (usually digital)
  MotorHL in1          : PWM
  MotorHL in2          : PWM
  MotorHL enable       : PWM (usually digital)
  MotorVR in1          : PWM
  MotorVR in2          : PWM
  MotorVR enable       : PWM (usually digital)
  MotorVL in1          : PWM
  MotorVL in2          : PWM
  MotorVL enable       : PWM  
*/

#include <VirtualWire.h>

double HL(int X, int Y)
{
  int x=max(X,-511);
  x=min(x,511);
  int y=max(Y,-511);
  y=min(y,511);
  
  double res=0;
  if (x+y>0)
    res=min(x+y,512);
  else
    res=max(x+y,-512);
  return res/512;
}

double HR(int X, int Y)
{
  int x=max(X,-511);
  x=min(x,511);
  int y=max(Y,-511);
  y=min(y,511);
  
  double res=0;
  if (y-x>0)
    res=min(y-x,512);
  else
    res=max(y-x,-512);
  return res/512;
}

//////////
// PINS //
//////////

// Inputs
int pinJoyAX         = A14; // analog
int pinJoyAY         = A13; // analog
int pinJoyAZ         = A15; // analog
int pinJoyBY         = A12; // analog

int pinMotHR1        = 2; // PWM
int pinMotHR2        = 3; // PWM
int pinMotHREn       = 4; // PWM
int pinMotVR1        = 5; // PWM
int pinMotVR2        = 6; // PWM
int pinMotVREn       = 7; // PWM

int pinMotHL1        = 8; // PWM
int pinMotHL2        = 9; // PWM
int pinMotHLEn       = 10; // PWM
int pinMotVL1        = 11; // PWM
int pinMotVL2        = 12; // PWM
int pinMotVLEn       = 13; // PWM

int pinLightL1       = 23; // digital
int pinLightL2       = 25; // digital
int pinLightLEn      = 27; // digital
int pinLightR1       = 22; // digital
int pinLightR2       = 24; // digital
int pinLightREn      = 26; // digital

// VARIABLES
int valJoyAX         = 0;
int valJoyAY         = 0;
int valJoyAZ         = 0;
int valJoyBY         = 0;
double valHL         = 0;
double valHR         = 0;
int valJoyAZPrev     = 0;
int calJoyAX         = 0;
int calJoyAY         = 0;
int calJoyBY         = 0;

///////////
// SETUP //
///////////

void setup()
{
  calJoyAX   = analogRead(pinJoyAX);
  calJoyAY   = analogRead(pinJoyAY);
  calJoyBY   = analogRead(pinJoyBY);
  
  pinMode(pinJoyAZ, INPUT_PULLUP);
  pinMode(pinLightL1, OUTPUT);
  pinMode(pinLightL2, OUTPUT);
  pinMode(pinLightLEn, OUTPUT);
  pinMode(pinLightR1, OUTPUT);
  pinMode(pinLightR2, OUTPUT);
  pinMode(pinLightREn, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Setup Remote Control");
}

//////////
// LOOP //
//////////

void loop()
{  
  /////////////////
  // READ INPUTS //
  /////////////////
  
  valJoyAX   = analogRead(pinJoyAX);
  valJoyAY   = analogRead(pinJoyAY);
  valJoyAZPrev = valJoyAZ;
  valJoyAZ   = digitalRead(pinJoyAZ);
  valJoyBY   = analogRead(pinJoyBY);

/*
  ///////////
  // DEBUG //
  ///////////
  
  Serial.println("-------------");
  Serial.print("Joystick A : "); 
  Serial.print(valJoyAX);
  Serial.print(", ");
  Serial.print(valJoyAY);
  Serial.print(", ");
  Serial.println(valJoyAZ);

  Serial.print("Joystick B : "); 
  Serial.println(valJoyBY);
*/


  ////////////
  // LIGHTS //
  ////////////
  
  if(valJoyAZ==0 && valJoyAZPrev==1 && lightState==0)
  {
    digitalWrite(pinLightL1,HIGH);
    digitalWrite(pinLightL2,LOW);
    analogWrite(pinLightLEn,255);
    
    digitalWrite(pinLightR1,HIGH);
    digitalWrite(pinLightR2,LOW);
    analogWrite(pinLightREn,255);
    
    lightState = 1;
  }
  else
  {
    if(valJoyAZ==0 && valJoyAZPrev==1 && lightState== 1)
    {  
      digitalWrite(pinLightL1,HIGH);
      digitalWrite(pinLightL2,LOW);
      analogWrite(pinLightLEn,0);
      
      digitalWrite(pinLightR1,HIGH);
      digitalWrite(pinLightR2,LOW);
      analogWrite(pinLightREn,0);
      lightState = 0;
    }
  }

  ////////////
  // MOTORS //
  ////////////
  
  // Due to the direction of the joysticks on the remote control, the X is Y and the Y is X
  valHL = HL(valJoyAY-calJoyAY, valJoyAX-calJoyAX);
  valHR = HR(valJoyAY-calJoyAY, valJoyAX-calJoyAX);
  
  // Motor HL
  if(valHL>0)
  {
    analogWrite(pinMotHL1,255);
    analogWrite(pinMotHL2,0);
    analogWrite(pinMotHLEn,255*valHL);
  }
  else
  {
    analogWrite(pinMotHL1,0);
    analogWrite(pinMotHL2,255);
    analogWrite(pinMotHLEn,-255*valHL);
  }
  
  // Motor HR
  if(valHR>0)
  {
    analogWrite(pinMotHR1,255);
    analogWrite(pinMotHR2,0);
    analogWrite(pinMotHREn,255*valHR);
  }
  else
  {
    analogWrite(pinMotHR1,0);
    analogWrite(pinMotHR2,255);
    analogWrite(pinMotHREn,-255*valHR);
  }
  
  // Motors VL and VR
  if(valJoyBY > calJoyBY + 10)
  {
    analogWrite(pinMotVL1,255);
    analogWrite(pinMotVL2,0);
    analogWrite(pinMotVLEn,min((valJoyBY-calJoyBY)/2,255));
    
    analogWrite(pinMotVR1,0);
    analogWrite(pinMotVR2,255);
    analogWrite(pinMotVREn,min((valJoyBY-calJoyBY)/2,255));
  }
  else
  {
    if(valJoyBY< calJoyBY-10)
    {
      analogWrite(pinMotVL1,0);
      analogWrite(pinMotVL2,255);
      analogWrite(pinMotVLEn,(calJoyBY-valJoyBY)/2);
      
      analogWrite(pinMotVR1,255);
      analogWrite(pinMotVR2,0);
      analogWrite(pinMotVREn,(calJoyBY-valJoyBY)/2);
    }
    else // switch off the vertical motors
    {
      analogWrite(pinMotVL1,0);
      analogWrite(pinMotVL2,0);
      analogWrite(pinMotVLEn,0);
      
      analogWrite(pinMotVR1,0);
      analogWrite(pinMotVR2,0);
      analogWrite(pinMotVREn, 0);
    }
  }
  
  delay(200);
}  

/*
Arduino code for the ROV.

Parts:
  DC Motor Z axis Right (MotorZR)
  DC Motor Z axis Left (MotorZL)
  DC Motor Horizontal plane Right (MotorHR)
  DC Motor Horizontal plane Left (MotorHL)
  Light Right (LightR)
  Light Left (LightL)
  Water detector (Wdetector1)
  Water detector (Wdetector2)
  1 LED (problem indicator)
  1 LED (battery state)

Inputs:
  Battery level        : int
  JoystickA X axis     : int
  JoystickA Y axis     : int
  JoystickA Z axis     : digital
  JoystickB Y axis     : int

Outputs:
  Light Right          : int
  Light Left           : int
  LED 1 (red)          : digital
  LED 2 (green)        : digital
  MotorHR in1          : digital
  MotorHR in2          : digital
  MotorHR enable       : PWM
  MotorHL in1          : digital
  MotorHL in2          : digital
  MotorHL enable       : PWM
  MotorZR in1          : digital
  MotorZR in2          : digital
  MotorZR enable       : PWM
  MotorZL in1          : digital
  MotorZL in2          : digital
  MotorZL enable       : PWM  
  
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
  {
    res=min(x+y,512);
  }
  else
  {
    res=max(x+y,-512);
  }
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
  {
    res=min(y-x,512);
  }
  else
  {
    res=max(y-x,-512);
  }
  return res/512;
}

//////////
// PINS //
//////////

// Inputs
int pinJoyAX         = 9; // analog
int pinJoyAY         = 8; // analog
int pinJoyAZ         = 53;
int pinJoyBX         = 1; // analog
int pinWDA           = 51;
int pinWDB           = 50;
int pinTensMon       = 0; // analog

// Outputs
int pinLEDPB         = 49;
int pinLEDPow        = 48;

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

int pinLightL1       = 23;
int pinLightL2       = 25;
int pinLightLEn      = 27;
int pinLightR1       = 22;
int pinLightR2       = 24;
int pinLightREn      = 26;



// VARIABLES
int valJoyAX         = 0;
int valJoyAY         = 0;
int valJoyAZ         = 0;
int valJoyBX         = 0;
int valWDA           = 0;
int valWDB           = 0;
int valTensMon       = 0;
int ledPBBlink       = 0;
int ledPowBlink      = 0;
double valHL         = 0;
double valHR         = 0;
int onboardLEDState  = 0;
int lightState       = 0;
int valJoyAZPrev     = 0;
int calJoyAX         = 0;
int calJoyAY         = 0;
int calJoyBX         = 0;

void setup()
{
  calJoyAX   = analogRead(pinJoyAX);
  calJoyAY   = analogRead(pinJoyAY);
  calJoyBX   = analogRead(pinJoyBX);
  
  pinMode(pinJoyAZ, INPUT_PULLUP);
  pinMode(pinWDA, INPUT);
  pinMode(pinWDB, INPUT);
  pinMode(pinLEDPB, OUTPUT);
  pinMode(pinLEDPow, OUTPUT);
  pinMode(pinLightL1, OUTPUT);
  pinMode(pinLightL2, OUTPUT);
  pinMode(pinLightLEn, OUTPUT);
  pinMode(pinLightR1, OUTPUT);
  pinMode(pinLightR2, OUTPUT);
  pinMode(pinLightREn, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Setup Remote Control");
}

void loop()
{  
  /////////////////
  // READ INPUTS //
  /////////////////
  
//  valJoyAX = map(analogRead(pinJoyAX),0,1023,-512,512);
  valJoyAX   = analogRead(pinJoyAX);
  valJoyAY   = analogRead(pinJoyAY);
  valJoyAZPrev = valJoyAZ;
  valJoyAZ   = digitalRead(pinJoyAZ);
  valJoyBX   = analogRead(pinJoyBX);
  valWDA     = digitalRead(pinWDA);
  valWDB     = digitalRead(pinWDB);
  valTensMon = analogRead(pinTensMon);

  ///////////
  // DEBUG //
  ///////////
//  
//  Serial.println("-------------");
//  Serial.print("Joystick A : "); 
//  Serial.print(valJoyAX);
//  Serial.print(", ");
//  Serial.print(valJoyAY);
//  Serial.print(", ");
//  Serial.println(valJoyAZ);
//
//  Serial.print("Joystick B : "); 
//  Serial.println(valJoyBX);




  ////////////
  // LIGHTS // OK
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
      //digitalWrite(pinLightL,LOW);
      //digitalWrite(pinLightR,LOW);
      digitalWrite(pinLightL1,HIGH);
      digitalWrite(pinLightL2,LOW);
      analogWrite(pinLightLEn,0);
      
      digitalWrite(pinLightR1,HIGH);
      digitalWrite(pinLightR2,LOW);
      analogWrite(pinLightREn,0);
      lightState = 0;
    }
  }
    
  /////////////////////
  // WATER DETECTION // 
  /////////////////////
  
  
  if(valWDA >0 || valWDB > 0)
  {
    // Water detected : make the LEDPB blink
    if(ledPBBlink < 0)
    {
      digitalWrite(pinLEDPB,LOW);
      ledPBBlink++;
    }
    else
    {
      if(ledPBBlink < 100)
      {
        digitalWrite(pinLEDPB,HIGH);
        ledPBBlink++;
      }
      else
      {
        ledPBBlink=-200;
      }
    }
    
  }
  else
  {
    // No problem
    digitalWrite(pinLEDPB,LOW);
  }
  
  ///////////////////
  // BATTERY STATE //
  ///////////////////
  
  //   x  (+)---,
  //   |       R1
  // U |        |----pinTensReg x
  //   |       R2               | Vout
  //   |  (-)---'----Gnd        |
  //
  // Vout = R1/(R1+R2) . U
  // U = 12 V
  // R2 = 68 Ohms
  // R1 = 47 Ohms
  // Vout ~ 4.9 V < 5 V  
  
  if(valTensMon> 3.5)
  {
    // Correct tension
    digitalWrite(pinLEDPow,HIGH);
  }
  else
  {
    // Low tension 
    if(ledPowBlink<0)
    {
      digitalWrite(pinLEDPow,LOW);
      ledPowBlink++;
    } 
    else
    {
      if(ledPowBlink<100)
      {
        digitalWrite(pinLEDPow,HIGH);
        ledPowBlink++;
      }
      else
      {
        ledPowBlink=-200;
      }
    }
  }


  ////////////
  // MOTORS // OK
  ////////////
  // (Check the directions)
  
  // Due to the direction of the joysticks on the remote control, the X is Y and the Y is X
  valHL = HL(valJoyAY-calJoyAY, valJoyAX-calJoyAX);
  valHR = HR(valJoyAY-calJoyAY, valJoyAX-calJoyAX);
  
//  Serial.print("Values : X=");
//  Serial.print(valJoyAY-calJoyAY);
//  Serial.print(", Y=");
//  Serial.println(valJoyAX-calJoyAX);
//  Serial.print("Motors : valHL=");
//  Serial.print(valHL);
//  Serial.print(", valHR=");
//  Serial.println(valHR);
//  
  
  // Motor HL // OK
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
  
  // Motors VL and VR // OK
  if(valJoyBX > calJoyBX + 10)
  {
    analogWrite(pinMotVL1,0);
    analogWrite(pinMotVL2,255);
    analogWrite(pinMotVLEn,min((valJoyBX-calJoyBX)/2,255));
    
    analogWrite(pinMotVR1,0);
    analogWrite(pinMotVR2,255);
    analogWrite(pinMotVREn,min((valJoyBX-calJoyBX)/2,255));
  }
  else
  {
    if(valJoyBX< calJoyBX-10)
    {
      analogWrite(pinMotVL1,255);
      analogWrite(pinMotVL2,0);
      analogWrite(pinMotVLEn,(calJoyBX-valJoyBX)/2);
      
      analogWrite(pinMotVR1,255);
      analogWrite(pinMotVR2,0);
      analogWrite(pinMotVREn,(calJoyBX-valJoyBX)/2);
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

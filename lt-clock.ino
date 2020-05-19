/*
 * FOSDEM lightning talks clock
 * created by Ghosty
 * 2017-01-28
 * This software is distributed under GPL v3 license
 */

unsigned long Watch, _micro, time = micros();
unsigned int Clock = 0, R_clock;
boolean Reset = false, Stop = false, Paused = false;
volatile boolean timeFlag = false;

// standard library
#include <SPI.h>
// add MD_Parola library to Arduino IDE
#include <MD_Parola.h>
// add MD_MAX72xx library to Arduino IDE
#include <MD_MAX72xx.h>
// git -C ~/Arduino/libraries/ clone https://github.com/rocketscream/Low-Power
#include <LowPower.h>

// Define the number of devices we have in the chain and the hardware interface
// NOTE: These pin numbers will probably not work with your hardware and may
// need to be adapted
#define MAX_DEVICES 4
#define CLK_PIN   13
#define DATA_PIN  11
#define CS_PIN    10

// Hardware SPI connection
MD_Parola P = MD_Parola(CS_PIN, MAX_DEVICES);
// Arbitrary output pins
// MD_Parola P = MD_Parola(DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

//initial countdown time
int M = 15; // minutes
int S = 0;  // seconds 

// pin for the buzzer
int BUZ_PIN = 9;

void setup(void)
{
  P.begin();
  pinMode (BUZ_PIN, OUTPUT);
  beep();
}

//declare reset function at address 0
void(* resetFunc) (void) = 0;

void beep()
{
  analogWrite (BUZ_PIN, 0);
  delay (100);
  analogWrite (BUZ_PIN, 255);
}

void loop()
{
  //init character array for displaying on the led matrix
  char Time[6] = "";
  // sleep 1 second
  LowPower.powerDown(SLEEP_1S, ADC_OFF, BOD_OFF);

  if(M==0 && S==1)
  {
    // when timer reaches 00:00 it should display stop and beep 3 times
    P.print(" STOP! ");
    beep();
    delay (100);
    beep();
    delay (100);
    beep();
    //turn off the led matrix to save power
    delay (5000);
    P.print("");
    // for loop testing purposes
    //delay (5000);
    //resetFunc();  //call reset
    // end loop testing
    exit(0);
  }
  else
  {
    //countdown
    S--;
    if(S<0 && M>0) //after 00 seconds subtract 1 minute and set seconds to 59 again 
    {
      M--;
      S=59;
    }
    if(M==1 && S == 0) //beep at 1 minute left
    {
      beep();
    }
    //reformat time to show double digits at all times
    sprintf(Time," %02d:%02d",M,S);
    // send it to the led matrix
    P.print(Time);
  }
}

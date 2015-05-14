/**
*
* This firmware is a software implementation of VOX vfs5 footswitch, 
* compatible with the VT/VT+ series of guitar amplifiers. 
* it's tested on a VT20+.
*
* Have a look to http://www.whitelamp.com/public/vox_vfs5.html for the 
* analog circuit reverse engineered from the original VOX footswitch.
*
* we generate a PWM on arduino's digital pin 5,
* setting the port to the highest possible frequency (~60kHz)
* and apply to it a simple dual pole low-pass filter to convert 
* the PWM signal in a continous analog voltage:
*
*	pin5 >------- 1.2KOhm ------ 1.2KOhm ------> out 
*	                         |              |
*	                        47nF           47nF
*	                        _|_            _|_
*                           ///            ///
*
*
* TODO: use 4 input switches for channel selection
* TODO: use a led (or two) to signal currently selected bank
* WARNING: the voltage provided by the amp drops to 4.25V when arduino is connected... see MAX_OUTV const
*
**/


#include <stdio.h>

#define dbg false

int I_BANK = 2;
int I_CH = 3;
int OUT = 5;

const double MAX_OUTV = 4.25f;

const double CONTINOUSG= 2.85f;
const double CONTINOUSR = 3.24f;

double table_bankG(int ch){
  switch(ch){
    case 1: return 2.33f;
    case 2: return 1.95f;
    case 3: return 1.32f;
    case 4: return 0.82f;
  }
}

double table_bankR(int ch){
  switch(ch){
    case 1: return 2.58f;
    case 2: return 2.12f;
    case 3: return 1.40f;
    case 4: return 0.85f;
  }
}

double table(boolean bank, int ch){
  if(bank)
    return table_bankG(ch);
  else
     return table_bankR(ch);
}

// the setup routine runs once when you press reset:
void setup() {                
  
  //Set timer2 freq (ports D9 & D10) to 30KHz
  //TCCR2B = TCCR2B & B11111000 | B00000001;   //set timer2 divisor to 1 for PWM frequency of 31372.55 Hz
  TCCR0B = ((TCCR0B & B11111000) | B00000001); //Set timer0 freq (ports D5 & D6) to 60KHz

  // initialize the digital pin as an output.
  pinMode(I_BANK, INPUT);
  pinMode(I_CH, INPUT);
  pinMode(OUT, OUTPUT);

  TCCR0B = ((TCCR0B & B11111000) | B00000001);

  if(dbg){
    Serial.begin(9600);  
    char s[120];
    sprintf(s,"debug active");
    Serial.println(s);
  }

}

char s[120];
boolean bank = true;
int ch = 1;
double v;

boolean new_bank_state = false;
boolean bank_state = false;
boolean new_ch_state = false;
boolean ch_state = false;
double continous = CONTINOUSG;

// the loop routine runs over and over again forever:
void loop() {
  if((new_bank_state = digitalRead(I_BANK)) != bank_state){
    bank_state = new_bank_state;
    if(bank_state){
      bank = !bank;
    }
  }
  
  if( (new_ch_state = digitalRead(I_CH)) != ch_state){
    ch_state = new_ch_state;
    
    if(ch_state){
      ch++;

      if(ch>4)
        ch=1;
    
      v = table(bank,ch) ;
  
    }
  }else {
      v = bank ? CONTINOUSG : CONTINOUSR;
  }
    
  int out = v*255/MAX_OUTV;  
  analogWrite(OUT,out);
  if(dbg){
    sprintf(s,"bank %d ",bank);
    Serial.print(s);
    sprintf(s,"ch %d ",ch);
    Serial.print(s);
    sprintf(s,"v=%d out=%d",(int)(v*100),out);
    Serial.println(s);
  }
    
}


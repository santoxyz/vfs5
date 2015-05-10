#include <stdio.h>

int I_BANK = 2;
int I_CH = 3;
int OUT = 10;

boolean dbg = true;

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
  // initialize the digital pin as an output.
  pinMode(I_BANK, INPUT);
  pinMode(I_CH, INPUT);
  pinMode(OUT, OUTPUT);

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
      v = bank ? CONTINOUSG : CONTINOUSR;
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
  }
    
  int out = v*270/5;  
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


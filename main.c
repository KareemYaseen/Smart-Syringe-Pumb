#define A 8
#define B 9
#define C 10
#define D 11
 
#define NUMBER_OF_STEPS_PER_REV 512

int inPin =A0;   // choose the input pin (for a pushbutton)
int val = 0; 


int inPinR = A1;   
int valR = 0;


volatile int flow_frequency; // Measures flow sensor pulses
// Calculated litres/hour
 float vol = 0.000,l_minute;
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 5, 4, 3, 7);
void flow () // Interrupt function
{
   flow_frequency++;
}

void setup(){
pinMode(A,OUTPUT);
pinMode(B,OUTPUT);
pinMode(C,OUTPUT);
pinMode(D,OUTPUT);



  
 pinMode(inPinR, INPUT);



   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   lcd.begin(16, 2);
   attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Water Flow Meter");
   lcd.setCursor(0,1);
   lcd.print("Circuit Digest");
   currentTime = millis();
   cloopTime = currentTime;

 
}

void write(int a,int b,int c,int d){
digitalWrite(A,a);
digitalWrite(B,b);
digitalWrite(C,c);
digitalWrite(D,d);
}

void onestepF(){
  

    write(1,0,0,1);
    delay(1);
    write(0,0,0,1);
    delay(1);
    write(0,0,1,1);
    delay(1);
    write(0,0,1,0);
    delay(1);
    write(0,1,1,0);
    delay(1);
    write(0,1,0,0);
    delay(1);
    write(1,1,0,0);
    delay(1);
    write(1,0,0,0);
    delay(1);
    
  } 
  
void onestepR() {
    write(1,0,0,0);
    delay(1);
    write(1,1,0,0);
    delay(1);
    write(0,1,0,0);
    delay(1);
    write(0,1,1,0);
    delay(1);
    write(0,0,1,0);
    delay(1);
    write(0,0,1,1);
    delay(1);
    write(0,0,0,1);
    delay(1);
    write(1,0,0,1);
    delay(1);
    
  }

  void onestepD() {
    write(1,0,0,0);
    delay(2);
    write(1,1,0,0);
    delay(2);
    write(0,1,0,0);
    delay(2);
    write(0,1,1,0);
    delay(2);
    write(0,0,1,0);
    delay(2);
    write(0,0,1,1);
    delay(2);
    write(0,0,0,1);
    delay(2);
    write(1,0,0,1);
    delay(2);
    
  }
  
void mStop(){
   write(0,0,0,0);
  }




void loop(){

   currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
    if(flow_frequency != 0){
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print(l_minute);
      lcd.print(" L/M");
      l_minute = l_minute/60;
      lcd.setCursor(0,1);
      vol = vol +l_minute;
      lcd.println("Vol:");
      lcd.print(vol);
      lcd.print(" L");
      flow_frequency = 0; // Reset Counter
      Serial.print(l_minute, DEC); // Print litres/hour
      Serial.println(" L/Sec");
      Serial.println(vol);
    }
    else {
      Serial.println(" flow rate = 0 ");
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Rate: ");
      lcd.print( flow_frequency );
      lcd.print(" L/M");
      lcd.setCursor(0,1);
      lcd.println("Vol:");
      lcd.print(vol);
      lcd.print(" L");
    }
   }

int i;
i=0;
while(i<NUMBER_OF_STEPS_PER_REV){
  // read input value
  valR = digitalRead(inPinR);
  val = digitalRead(inPin);
    if (val == LOW) {         // check if the input is HIGH (button released)
onestepF();
}
  
   if (valR == LOW) {
onestepR();
}

  else {
  
mStop();  
}  
i++;
}


}

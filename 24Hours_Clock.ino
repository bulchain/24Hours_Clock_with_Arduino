#include "SevenSegmentExtended.h"

#define CLK 4
#define DIO 5
#define MIN_BUTT 7 //Button for time adjustment by increase Minute until reach desire time
#define mil2sec 60000 

SevenSegmentExtended display(CLK, DIO);

unsigned int min = 0;
unsigned int last_min = min;
unsigned long last_run = 0;
unsigned long last_blink = 0;


void setup() {
  pinMode(MIN_BUTT, INPUT);
  Serial.begin(9600);

  display.begin();
  display.setBacklight(100);
  display.setColonOn(true);
  display.print("8888"); //To test whether all LED segments are good.
  delay(1000);
  display.print("0000");
}

void loop() {
  unsigned long this_time = millis();
  
  //blink the colon to show the heartbeat
  if ((this_time - last_blink) >= 500) { 
    display.setColonOn(!display.getColonOn());
    last_blink = this_time;
  }

  // Check if ONE Minute is met then increase min value by 1
  if ((this_time - last_run) >= mil2sec){
    min++;
    last_run += mil2sec;
  }
  
  if (digitalRead(MIN_BUTT)) {
    Serial.println("Minute adjust Button is pressed.");
    min++;
    delay(150);
  }
   
  if (min == 1440){//End of day 
    min = 0;
  }
  
  show_digit(min); //Keep this every loop
  
}

void show_digit(unsigned int dec_in){
  unsigned int h = dec_in/60;
  unsigned int m = dec_in - (h*60);
  String show = "";
  
  //working on 2 digit hours
  if (h >= 10) {show = String(h);}
  //if h one digit need to put blank " " in front
  else if (h != 0) {show = " " + String(h);}
  // when h = 0 show 2 Zero
  else {show = "00";}

  //working on 2 digit minutes
  if (m >= 10) {show += String(m);} // m = 10 to 59
  else if (m >= 1) {show += "0" + String(m);} // m = 1 to 9
  else {show += "00";} // m = 0
  
  //ready to show
  display.print(show);
}

/*
 * Track Scale Simulator
 * 
 * This sketch simulates a Railroad track scale.
 * 
 * The sketch outputs data to a serial port in the same
 * format that the Boulder Creek Engingineering Track Scale 
 * (which is no longer available) sends data to the serial port.
 * 
 * This version of the sketch has no display, only output to 
 * the serial port.
 * 
 */

// define the input pin for the weight trigger.
// this should be either a button or an optical sensor
// that will trigger when there is a car on the scale track.
#define WEIGHT_PIN 2

// Active and inactive are aliases to high and low
#define DETECTORACTIVE LOW
#define DETECTORINACTIVE HIGH

// weight range data
typedef struct __weightrange {
  char range;
  long min_weight;
  long max_weight;
} weightrange;

weightrange A = {'A',50000L,200000L};  // loads
weightrange B = {'B',40000L,50000L}; // empties
weightrange C = {'C',75000L,85000L}; // scale test cars

weightrange current;

// serial output routines

//Output at startup and when the range is changed
void newRangeSelected(weightrange selected){
  // print out [R X min:max] where X is the range letter.
  Serial.print("[R ");
  Serial.print(selected.range);
  Serial.print(" ");
  Serial.print(selected.min_weight);
  Serial.print(":");
  Serial.print(selected.max_weight);
  Serial.println("]");
}

//Output the current weight
void sendWeightReport(weightrange selected,long weight){
  // print out [W X weight] where X is the range letter
  Serial.print("[W ");
  Serial.print(selected.range);
  Serial.print(" ");
  Serial.print(weight);
  Serial.println("]");
}


long calculate_weight(weightrange selected){
  return(random(selected.max_weight-selected.min_weight)+selected.min_weight);
}

void setup() {
  Serial.begin(9600);
  current = A; // set the default to loads.
  newRangeSelected(current);
  pinMode(WEIGHT_PIN,INPUT);
  randomSeed(analogRead(0));
}

void loop() {
  // put your main code here, to run repeatedly:
  if( digitalRead(WEIGHT_PIN)==DETECTORACTIVE){
    long weight = calculate_weight(current);
    sendWeightReport(current,weight);
    delay(100);
    while(digitalRead(WEIGHT_PIN)==DETECTORACTIVE);
    delay(100);
  }

}

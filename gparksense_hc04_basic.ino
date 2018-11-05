/*
* System Installed in Garage currently
*/
//System 1
int trigPin = 12;
int echoPin = 13;
int clockPin = 11;  
int latchPin = 10; 
int dataPin = 9;

bool awakeMode = true;
int parkSense = 1;
int ticks = 0;
int x = 0;
int awakeTicks = 0;

//LED Light Patterns to shift out
byte possible_patterns[9] = {
B00000000,
B00000001,
B00000011,
B00000111,
B00001111,
B00011111,
B00111111,
B01111111,
B11111111,
};
int proximity=0;
long duration;
double distance;

void setup() {
  //Serial Port
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(0, OUTPUT);
  Serial.println("Setup Complete");
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(latchPin, LOW);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration*0.034/2/2.54;
  
  if(parkSense > 199)
  {
    awakeMode = false;
  }
  else
  {
    awakeMode = true;
    Serial.print(distance);
    Serial.println(" in");
    Serial.print(parkSense);
    Serial.println(" ct");
    Serial.print(proximity);
    Serial.println(" px");
  }

  if(distance > 30)
  {
    proximity = 0; //Off
  }
  else if(distance <= 30 && distance > 25)
  {
    proximity = 1; //1 Green
  }
  else if(distance <= 30 && distance > 20)
  {
    proximity = 2; //2 Green
  }
  else if(distance <= 20 && distance > 10)
  {
    proximity = 3; //3 Green
  }
  else if(distance <= 15 && distance > 7)
  {
    proximity = 4; //1 Yellow
  }
  else if(distance <= 7 && distance > 5)
  {
    proximity = 5; //2 Yellow
  }
  else if(distance <= 5 && distance > 3)
  {
    proximity = 6; //1 Red
  }
  else if(distance <= 3 && distance > 1.75)
  {
    proximity = 7; //2 Red
  }
  else if(distance <= 1.75)
  {
    proximity = 8; //3 Red
  }
  
  ticks = ticks + 1;
  x = ticks % 300;
  if(x == 0)
  {
    awakeTicks = awakeTicks + 1;
    Serial.print(awakeTicks);
    Serial.println(" mins awake");
  }

  if (proximity <= 0){
    proximity=0;
    parkSense=1;
  }
  else if (proximity >= 1 && proximity <= 5){
    parkSense=1;
  }
  else if (proximity >= 6 && proximity <= 8){
    if(parkSense < 200)
    {
    parkSense = parkSense + 1;
    }
  }

  if(awakeMode)
  {
    shiftOut(dataPin, clockPin, MSBFIRST, possible_patterns[proximity]);
    digitalWrite(latchPin, HIGH);
  }
  else
  {
    shiftOut(dataPin, clockPin, MSBFIRST, possible_patterns[0]);
    digitalWrite(latchPin, HIGH);  
  }

  delay(200);
}

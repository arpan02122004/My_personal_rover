// Pins for ultrasonic sensor
const int triggerPin = 6;
const int echo = 5;
const int buzzer = 10;
// vcc to 5v buzzer -ve to gnd and ultrasonic gnd to same gnd 
long duration;
int distance;


void setup(){
pinMode(triggerPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echo, INPUT); // Sets the echoPin as an Input
pinMode(buzzer, OUTPUT);
Serial.begin(9600); // Starts the serial communication
 }


void loop(){
digitalWrite(triggerPin, LOW);
delayMicroseconds(2);
digitalWrite(triggerPin, HIGH);
delayMicroseconds(10);
digitalWrite(triggerPin, LOW);
duration = pulseIn(echo, HIGH);
distance = duration*0.034/2;
if (distance <= 10){
  digitalWrite(buzzer, HIGH);
}
else{
  digitalWrite(buzzer, LOW);
 }
Serial.print("Distance: ");
Serial.println(distance);
 }

#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
HUSKYLENS huskylens;
SoftwareSerial mySerial(3, 4);
void printResult(HUSKYLENSResult result);
int RightMotor_E_pin = 9;
int LeftMotor_E_pin = 10; 
int trigPin = 6;
float distance;
int echoPin = 5;
int flag_detour;
int check_l =0; 
int u_check=0; 
int check_r = 0; 
int E_carSpeed = 150; 
void setup() {
    Serial.begin(115200);
    mySerial.begin(9600);
    TCCR1B = TCCR1B & B11111000 | B00000101;
    pinMode(echoPin, INPUT);   // echoPin 입력
    pinMode(trigPin, OUTPUT);  // trigPin 출력
    pinMode (9, OUTPUT);
    pinMode (10, OUTPUT);
    pinMode (8, OUTPUT);
    pinMode (11, OUTPUT);
    pinMode (12, OUTPUT);
    pinMode (13, OUTPUT);
    while (!huskylens.begin(mySerial))
    {
        Serial.println(F("fail"));
        delay(100);
    }
}
void loop() 
{
  long duration, distance;
  digitalWrite(trigPin, HIGH);                        // trigPin에서 초음파 발생(echoPin도 HIGH)
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);                  // echoPin 이 HIGH를 유지한 시간을 저장 한다.
  distance = ((float)(340 * duration) / 1000) / 2; 
  Serial.print("distance:");                          // 물체와 초음파 센서간 거리를 표시
  Serial.print(distance);
  Serial.println(" mm");
  delay(100);
    if (!huskylens.request())   
        Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));

    else if(!huskylens.isLearned()) {
        Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
        stop();
    }
    else if(!huskylens.available()) {

        if(distance < 300){
              
          forward();       
               
         }
         else{
          forward(); 
          delay(700); 
          
          stop();
          delay(1000); 
           if(check_l==1){
            right(); 
            delay(1000); 
            check_l=0; 
             }
            else if(check_r==1){
                left(); 
                delay(1000); 
                check_r=0; 
            }
         }
         
Serial.println(F("No block or arrow appears on the screen!"));


}
    else
    {
        Serial.println(F("###########"));
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            printResult(result);
            if(distance < 200){
                detour(result); 
         
            }
            else{
             driveBot(result);
            }
        }    
    }
}
void printResult(HUSKYLENSResult result){
    if (result.command == COMMAND_RETURN_BLOCK){
        Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);       
    }
    else if (result.command == COMMAND_RETURN_ARROW){
        Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
    }
    else{
        Serial.println("Object unknown!");
    }
}
void detour(HUSKYLENSResult result) {
  stop(); 
  delay(2000);   
   if(result.xCenter<=150)
   {
     check_l =1; 
     left();
     delay(1000); 
     stop(); 
     delay(2000);
   }
   else if(result.xCenter>150)
  {
    check_r=1;              
    right();
    delay(1000); 
    stop(); 
    delay(2000);
  }   
}



void driveBot(HUSKYLENSResult result)
{
  if(result.width<=47)
  {
      forward();
  }
  else if(result.xCenter<=65)
  {
      left();
  }
  else if(result.xCenter>=235)
  {
      right();
  }
  else if(result.width>47&&result.width<70)
  {
    stop();
  }
  else if (result.width>=70){
    backward();
  }
}


void stop()
{
digitalWrite(8, LOW);
digitalWrite(11, LOW);
digitalWrite(12, LOW);
digitalWrite(13, LOW);
Serial.println("Stop");
}
void right()
{
  
analogWrite(LeftMotor_E_pin,150);
digitalWrite(12, HIGH);
digitalWrite(13, LOW);
Serial.println(" Rotate Left");
}
void left()
{
analogWrite(RightMotor_E_pin,150 );     
digitalWrite(8, HIGH);
digitalWrite(11, LOW);
Serial.println(" Rotate Right"); 
}



void forward()
{
analogWrite(RightMotor_E_pin,130 );     
analogWrite(LeftMotor_E_pin,130);
digitalWrite(8, HIGH);
digitalWrite(11, LOW);
digitalWrite(12, HIGH);
digitalWrite(13, LOW);

Serial.println("Forward");
}

void backward()
{
analogWrite(RightMotor_E_pin,130 );     
analogWrite(LeftMotor_E_pin,130);
digitalWrite(8, LOW);

digitalWrite(11, HIGH);
digitalWrite(12, LOW);
digitalWrite(13, HIGH);
Serial.println("Backward");
}


#include <Wire.h>  // Wire library - used for I2C communication

int ADXL345 = 0x53; // The ADXL345 sensor I2C address
float X_out, Y_out, Z_out;  // Outputs

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600); 
  Wire.begin(); 
  Wire.beginTransmission(ADXL345); // Start communicating with the device 
  Wire.write(0x2D); // Access/ talk to POWER_CTL Register - 0x2D
  Wire.write(8);  // Enable measurement
  Wire.endTransmission();// (8dec -> 0000 1000 binary) Bit D3 High for measuring enable 
  delay(10);
  Serial.println("Welcome,please input the length of password.");
}
void loop() {
  char num=0;
  int len=0;
  char val[6];//password you set
  int count=3;//chances to unlock
  char input[6];//store the passward you try
  char gesture;//store the gesture you determine
  bool check;//check the passward input correct.
//================================set the lock gestures==========================================//
//set the length of password.
  if(Serial.available()>0){
    num = Serial.read();
    if ((num >= '4') && (num<= '8')){
      Serial.print("your length of lock is ");
      Serial.println(num);
      len = int(num-48);
      delay(300);
//set the passward.
      for(int i=0;i<len;i++){
        gesture = get_gesture();
        if(gesture){
          val[i]=gesture;
          Serial.println("successfully input");
          delay(600);
        } 
        else{
          i--;
        }
      }
      Serial.print("your passward is: ");
      for(int i=0;i<len;i++){ 
        Serial.print(val[i]);
      }
      Serial.print('\n');
      Serial.println("The lock gesture setting finish!"); 
      delay(120); 
      Serial.println("let's start to unlock it");
    } 
//===============================obtain the unlock gestures=========================================//
  while(count){
    delay(800);
    count--;
    int i,j;
    for(i=0;i<len;i++){
      gesture = get_gesture();
      if(gesture){
        input[i]=gesture;
        Serial.println("successfully input");
        delay(1200);
      }
      else{
        i--;
      }
    }
    Serial.print("The passward you input is: ");
    for(i=0;i<len;i++){
      Serial.print(input[i]);
    }
    Serial.print('\n');
//==============================check the input sequence ============================================//
    for(i=0;i<len;i++){
      check = true;
      if(val[i]!=input[i]){
        if(count){
        Serial.println("your password is wrong!please try again!");
        check = false;
        break;
        }
        else{
          Serial.println("your password is wrong!No more times");
          digitalWrite(LED_BUILTIN, LOW);
          check = false;
          Serial.println("let's reset your unlock password,please input the length of your passward");
          break;
        }
        
      }
    }
    if(check){
      Serial.println("your password is right!Successfully unlock!");
//LED blink indicate unlock successfully.
      for(i=0;i<4;i++){
        digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
        delay(500);                       // wait for a second
        digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
        delay(500);
      }
      Serial.println("let's reset your unlock password,please input the length of your passward");//do it again.
      break;
    } 
  } 
 }
}
//========================================function to read gesture====================================//
char get_gesture(){
//Read acceleromter data.
  Wire.beginTransmission(ADXL345);
  Wire.write(0x32); // Start with register 0x32 (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(ADXL345, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  X_out = ( Wire.read()| Wire.read() << 8);
  Y_out = ( Wire.read()| Wire.read() << 8);
  Z_out = ( Wire.read()| Wire.read() << 8);
  delay(300);
//Determine the gesture.
  bool judge= true;//if determine the gesture we set return true,else return false to wait.
  if(X_out>210&&X_out<270)
  {
    return 'F';//The gesture is palm forward.
    judge = false;
  }
  else if(X_out>65200&&X_out<65400)
  {
    return 'B';//The gesture is palm back.
    judge = false;
  }
  if(Y_out>210&&Y_out<270)
  {
    return 'L';//The gesture is palm left.
    judge = false;
  }
  else if(Y_out>65200&&Y_out<65400)
  {
    return 'R';//The gesture is palm right.
    judge = false;
  }
  if(Z_out>210&&Z_out<270)
  {
    return 'U';//The gesture is palm up.
    judge = false;
  }
  else if(Z_out>65200&&Z_out<65400)
  {
    return 'D';//The gesture is palm down.
    judge = false;
  }
  if(judge){
    return 0;//there are no gesture we can determine.
  } 
}

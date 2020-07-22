/*
   G4G - Sammie and the Cookes - Summer 2020 - Laser Turret Project
*/
#include<Wire.h>
#include<Servo.h>

Servo servo, servo2;
int angleX, angleY, angleZ, mpuX, mpuY, mpuZ;
const int ACCELEROMETER_MIN = -17650;
const int ACCELEROMETER_MAX = 18500;
const int SERVO_MAX = 3600;
const int SERVO_MIN = 100;

  /*
   * Sets servo physical range values and MPU 6050 for receiving input.
   * Connect MPU 6050(0x68) to arduino and display on 9600 baud.
   * Set serial headers for data from servo and MPU 6050 on 9600 baud.
   */
void setup(){
  servo.attach(9, 0, 2400);   
  servo2.attach(10, 0,2400);

  Wire.begin();
  Wire.beginTransmission(0x68); //MPU address
  Wire.write(0x6B); //PWR_MGMT_1 address
  Wire.write(0); //Wakes up MPU   
  Wire.endTransmission();
  
  Serial.begin(9600); 
  Serial.println("Accel X:\tServo:"); //Serial headers
}


void loop(){
  Wire.beginTransmission(0x68);
  Wire.write(0x3B); //ACCEL_XOUT_H internal address 
  Wire.requestFrom(0x68, 6); //Request data from x-axis accel registers
  //X direction
  mpuX = Wire.read()<<8; //High 3B
  mpuX |= Wire.read(); //Low 3C
  //Y direction
  mpuY = Wire.read()<<8;
  mpuY |= Wire.read();
  
  endReading();

  angleX = mpuX;
  angleY = mpuY;

  //Inverses MPU value to servo angle  
  angleX = map(angleX, ACCELEROMETER_MIN, ACCELEROMETER_MAX, SERVO_MAX, SERVO_MIN); 
  angleY = map(angleY, ACCELEROMETER_MIN, ACCELEROMETER_MAX, SERVO_MAX, SERVO_MIN);
  
  servo.writeMicroseconds(angleX);
  servo2.writeMicroseconds(angleY);

  //prints so we can see the input
  printToSerial();
}


void printToSerial(){
  Serial.print(mpuX); //Accel value
  Serial.print("\t");
  Serial.print(mpuY);
  Serial.print("\t");
  Serial.print(angleX); //Servo value
  Serial.print("\t");
  Serial.print(angleY);
  Serial.print("\t");
  Serial.print(angleZ);
  Serial.println();
  delay(10);
}

void endReading(){
  mpuZ = Wire.read()<<8;
  mpuZ |= Wire.read();
  Wire.endTransmission();
}

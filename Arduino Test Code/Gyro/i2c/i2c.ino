#include "mpu6500.h"
#include "DGMotor.h"
#include "HardwareSerial.h"
#include "Arduino.h"

const int LOOP_TIME = 10; //10 seconds
const int DT = .01; //100hz delta t
const int ALPHA = .98; //alpha for low/high pass filter
int rad_x = 0; //update radians in x

int Td = 1000; // target distance
int Tp = 80; // Target power - percentage of max power of motor (power is also known as 'duty cycle' ) 
int Kp = 3; //  the Constant 'K' for the 'p' proportional controller


/* Mpu6500 object */
bfs::Mpu6500 imu;
#include "DGMotor.h"

DGMotor leftMotor(Serial1, 1);
DGMotor rightMotor(Serial2, 1);

void setup() {

  /* Serial to display data */
  Serial.begin(115200);
  while(!Serial) {}
  /* Start the I2C bus */
  Wire.begin();
  Wire.setClock(400000);
  /* I2C bus,  0x68 address */
  imu.Config(&Wire, bfs::Mpu6500::I2C_ADDR_PRIM);
  
  leftMotor.begin(115200);
  rightMotor.begin(115200);
  Serial.println("meow");
  delay(1000); // Allow motors to initialize


  /* Initialize and configure IMU */
  if (!imu.Begin()) {
    Serial.println("Error initializing communication with IMU");
    while(1) {}
  }
  /* Set the sample rate divider */
  if (!imu.ConfigSrd(19)) {
    Serial.println("Error configured SRD");
    while(1) {}
  }


}

void loop() {

   update();
   int correction = Kp * rad_x * -1; 
   
   int powerA = Tp + correction;
   int powerB = Tp - correction;  

   leftMotor.setMotorSpeed(powerA);
   rightMotor.setMotorSpeed(powerB); 
   
    char* buffer = "error " + rad_x + "; correction " + correction + "; powerA " + powerA + "; powerB " + powerB; 
    Serial.println(buffer);
   driveBreak();
}

void update(){
  //change later below
  int Ax = 0;
  int Ay = 0;
  int Az = 0;
  int Gx = 0;
  //
  
  if(imu.Read()){
    Ax = imu.accel_x_mps2();
    Ay = imu.accel_y_mps2();
    Az = imu.accel_x_mps2();
    Gx = imu.gyro_x_radps();
  }
  
  int XdegA = atan2(Ax, sqrt((Ay*Ay) + (Az*Az)));
  
  rad_x = ALPHA*(rad_x+(Gx*DT)) + (1-ALPHA)*(XdegA); //calculate rad_x
}

void driveBreak() {
  leftMotor.brake();
  rightMotor.brake();
}
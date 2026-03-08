#include "mpu6500.h"
#include "PID_v1.h"


const int LOOP_TIME = 10; //10 seconds
const int DT = .01; //100hz delta t
const int ALPHA = .98; //alpha for low/high pass filter
int rad_x = 0; //update radians in x

/* Mpu6500 object */
bfs::Mpu6500 imu;

double Setpoint, Input, Output;
double Kp=2, Ki=5, Kd=1;
PID motPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

void setup() {

  /* Serial to display data */
  Serial.begin(115200);
  while(!Serial) {}
  /* Start the I2C bus */
  Wire.begin();
  Wire.setClock(400000);
  /* I2C bus,  0x68 address */
  imu.Config(&Wire, bfs::Mpu6500::I2C_ADDR_PRIM);
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
  

  
  motPID.SetOutputLimits(-255, 255);
  motPID.SetMode(AUTOMATIC);

}

void loop() {
  // make demo program that inputs acceleration values
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
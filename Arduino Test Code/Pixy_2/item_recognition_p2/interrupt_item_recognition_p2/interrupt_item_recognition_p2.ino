//NOTE:
//Object should be 3.5 cm away from object measures from dimaeter to pixy (not the camera, but to the board itself)

#include <avr/io.h>
#include <avr/interrupt.h>
#include <Servo.h>
#include <Pixy2SPI_SS.h>


//Pixy object 
Pixy2SPI_SS pixy;
//Servo object
Servo myservo;
//Timer used for pixy interrupts
IntervalTimer pixyTimer;

volatile uint16_t signature = 0;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");

  pinMode(8, OUTPUT);
  SPI.begin();
  SPI.usingInterrupt(128);
  SPI.beginTransaction(SPISettings(PIXY_SPI_CLOCKRATE, MSBFIRST, SPI_MODE3));

  myservo.attach(9);

  pixy.init();
  //set lamp to max for best item detection
  pixy.setLamp(255, 255);

  //Interval timer used for sampling the pixy camera every 20ms (60fps = 16.7ms)
  pixyTimer.priority(128);
  pixyTimer.begin(timerISR, 20000);

}

//Timer ISR that will read the pixy camera and will change the signature if a ball is detected
void timerISR(){
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks > 0){
    signature = pixy.ccc.blocks[0].m_signature;
  }
}

void loop()
{ 
  SPI.beginTransaction(SPISettings(PIXY_SPI_CLOCKRATE, MSBFIRST, SPI_MODE3));
  //Switch statment with a case for each ball color (1-red, 2-white, 3-blue)
  switch (signature) {
  //Red ball detected
  case 1:
    Serial.println("Red ball detected");
    pixy.ccc.blocks[0].printInfo();
    myservo.write(50);
    break;
  //White ball detected
  case 2:
    Serial.println("White ball detected");
    pixy.ccc.blocks[0].printInfo();
    myservo.write(100);
    break;
  //Blue ball detected
  case 3:
    Serial.println("Blue ball detected");
    pixy.ccc.blocks[0].printInfo();
    myservo.write(150);
    break;

  default:  
    break;
  }
  SPI.endTransaction();
}

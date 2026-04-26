// pixy 2 demo program
// modified from hello_world pixy provided program.
#include <Servo.h>
#include <Pixy2SPI_SS.h>

// This is the main Pixy object 
Pixy2SPI_SS pixy;

Servo myservo;

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  myservo.attach(9);
  pixy.init();
  //set lamp to max for best item detection
  pixy.setLamp(255, 255);
  
}

//NOTE:
// Object should be 3.5 cm away from object measures from dimaeter to pixy (not the camera, but to the board itself)

void loop()
{ 
  int i; 
  // grab blocks!
  pixy.ccc.getBlocks();
  
  // If there are detected blocks, print them
  if (pixy.ccc.numBlocks > 0)
  { 
    //Find area and age of block
    uint16_t area = pixy.ccc.blocks[0].area();
    uint16_t age = pixy.ccc.blocks[0].m_age;
    
    // if the area of the block is above a certain threshold, read the signature
    if(area > 4000) { 
      // first item in pixy.ccc.blocks (always has the most area out of the blocks found)
      // printInfo is a modification to the pixy2 library. It checks the signature (1-3.
      // each of which is pre-defined as a certain golf ball color) and then prints which
      // signature it is.
      //pixy.ccc.blocks[0].printInfo();
      uint16_t signature = pixy.ccc.blocks[0].m_signature;
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
    }
    
  }  
}


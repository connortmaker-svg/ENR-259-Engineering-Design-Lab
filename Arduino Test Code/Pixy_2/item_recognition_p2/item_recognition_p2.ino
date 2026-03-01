// pixy 2 demo program
// modified from hello_world pixy provided program.

#include <Pixy2.h>

// This is the main Pixy object 
Pixy2 pixy;



void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  
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
  if (pixy.ccc.numBlocks)
  { 
    // if the area of the block is above a certain threshold, print the color of the item. 
    uint16_t area = pixy.ccc.blocks[0].area();
    if(area > 40000)
      // first item in pixy.ccc.blocks (always has the most area out of the blocks found)
      // printInfo is a modification to the pixy2 library. It checks the signature (1-3.
      // each of which is pre-defined as a certain golf ball color) and then prints which
      // signature it is.
      pixy.ccc.blocks[0].printInfo();
    }
  }  



// from adafruit ssd1306 library::
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 32  // OLED display height, in pixels

#define RED 1
#define BLUE 2
#define WHITE 3

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);

  // Wait for display
  delay(500);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  delay(2000);  // Pause for 2 seconds

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // example code
  display.setCursor(10, 0);
  display_vacuum_speed(1000);
  display.setCursor(10, 10);
  display_color_detected(1);
  display.setCursor(10, 20);
  display_battery_level();



  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// input: pulse (us pulse)
// output: display to screen
// Display the current
void display_vacuum_speed(float pulse) {
  float conversion_factor = 0.06;
  float conversion_intercept = 20;
  float level;
  level = floor(pulse * conversion_factor - conversion_intercept);

  display.println("Motor level: ");
  display.setCursor(85, 0);
  display.println(level);
  display.setCursor(100, 0);
  display.display();
}


// display color detected by pixy
void display_color_detected(int color) {

  display.println("Color detected: ");
  display.setCursor(100,10);
  switch (color) {
    case RED:
      display.println("R");
      break;
    case BLUE:
      display.println("B");
      break;
    case WHITE:
      display.println("W");
      break;
    default:
      display.println("N");
      break;
  }

  display.display();
}

// display current battery percentage
void display_battery_level() {
  display.println("Batt level: 0%");
  display.display();
}
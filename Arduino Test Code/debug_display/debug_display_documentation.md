# debug_display
Display debug information on SSD1306 LCD. 

# Functions
## display_vacuum_speed
``` c
void display_vacuum_speed(float pulse)
```
### Description
`pulse` is microsecond pulse used to control vacuum motor. It is automatically converted into a percentage linearly using the equation: `level = 0.06 * pulse + 20`.
### Example
``` c
display_vacuum_speed(1000);
// Displays "Motor level: 40.00"
```
## display_color_detected
``` c
void display_color_detected(int color)
```
### Description
Uses a switch case statement where `color` is an integer representing Pixy output signatures. Currently each possible color is represented by a `#define` where:
- `RED` = 1
- `BLUE` = 2
- `WHITE` = 3 
- `default`  = (invalid color, any integer not from 1-3)
Each color is associated with a single printed character shown on the display. 
- `R` = red
- `B` = blue
- `W` = white
- `N` = no color detected
### Example
``` c
display_color_detected(3);
// Displays "Color detected: W" 
```

## display_battery_level
``` c
void display_battery_level(float percentage)
```
### Description
`percentage` (a percentage multiplied by 100) is the current battery level which is displayed to the screen.
### Example
``` c 
display_battery_level(20)
// Displays "Batt level: 20.00"
```

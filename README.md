# ENR-259-Engineering-Design-Lab

## Schedule Visualizer
The `ScheduleVisualizer` folder contains a Python program designed to help our team facilitate finding common meeting times. It allows users to input their availability and provides a visual heatmap of the team's schedule. This tool was created briefly to assist with quick coordination.

## Turbine Control Library
The `TurbineControl` folder contains an Arduino library for controlling a turbine ESC/motor.

### Usage
- **Include**: `#include <TurbineControl.h>`
- **Initialize**: `TurbineControl turbine(pin);`
- **Methods**:
    - `turbine.begin()`: Attaches the motor.
    - `turbine.arm()`: Sends arming signal (1000us).
    - `turbine.turnOn()`: Sends max speed signal (2000us).
    - `turbine.turnOff()`: Sends stop signal (1000us).

## Test Code
For testing scripts, prototypes, and hardware verification code (Sensor tests, Roomba wheels, etc.), please see the [TestCode/README.md](TestCode/README.md).

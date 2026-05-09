# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a robotics project for autonomous "PAMI" robots built on the Arduino Nano ESP32 platform using PlatformIO. The robots navigate a field using stepper motors and ultrasonic sensors, following predefined waypoints for a competition that lasts 15 seconds after an 85-second wait period.

## Build & Development Commands

### Building and Uploading
```bash
# Build the project
pio run

# Upload to Arduino Nano ESP32
pio run --target upload

# Monitor serial output (9600 baud)
pio device monitor

# Build and upload in one command
pio run --target upload && pio device monitor
```

### Cleaning
```bash
# Clean build files
pio run --target clean
```

## Code Architecture

### Multi-Core FreeRTOS Design

The system uses both cores of the ESP32 with FreeRTOS tasks:

- **Task1 (Core 0)**: Sensor monitoring, obstacle avoidance logic, and competition timer management
- **Task2 (Core 1)**: Main navigation logic, waypoint following, and odometry

Both tasks wait for the `tirette` (starting cord) to be pulled before beginning execution.

### Configuration-Driven System

**All robot-specific configuration must be set in:**
- [lib/config_robots/config_robots.c](lib/config_robots/config_robots.c) - Waypoint paths and behavior
- [lib/config_robots/config_robots.hh](lib/config_robots/config_robots.hh) - Physical parameters and calibration

Select which PAMI to compile for by uncommenting the appropriate `#define PAMI_X` in [config_robots.hh](lib/config_robots/config_robots.hh:36-42).

Each PAMI has calibrated parameters:
- `DIAMETRE_ROUE`: Wheel diameter affecting all movement calculations
- `COEF_DROIT`: Correction coefficient for asymmetric wheels (affects straight line accuracy)
- `ENTRE_AXE`: Distance between wheels (affects rotation accuracy)
- `X_POSITION_START`, `Y_POSITION_START`, `TETA_POSITION_START`: Initial position and orientation
- Waypoints array with `{x, y}` coordinates in millimeters
- `avoidance[]`: Boolean array enabling/disabling obstacle avoidance per waypoint
- `rpms[]`: Speed (RPM) for each waypoint segment

### Coordinate System

The field uses a fixed coordinate system:
- **0°**: Direction toward the kitchen
- **Positive rotation**: Clockwise (negative rotations accepted)
- **Movement**: All distances in millimeters (negative values accepted)
- **Origin**: Bottom-left corner of the 3000x2000mm field

Team color selection (blue/yellow) via `bouton_equipe` applies field symmetry transformations to all waypoints and the starting position.

### Movement API

Located in [lib/mouvement/move.h](lib/mouvement/move.h) and [lib/mouvement/move.cpp](lib/mouvement/move.cpp):

- `straight(mm)`: Move forward/backward (negative values allowed)
- `rotation(degrees)`: Rotate relative to current orientation (positive = clockwise)
- `orientation(angle_absolue)`: Rotate to absolute field angle (0-360°)
- `go_to(x_mm, y_mm)`: Navigate to waypoint using closed-loop positioning

The movement library maintains global odometry state:
- `x_position`, `y_position`: Current position in mm
- `teta_actuelle`: Current absolute orientation (0-360°)

### Stepper Motor Control

Uses custom BasicStepperDriver library ([lib/stepper_master/](lib/stepper_master/)) with:
- 200 steps/revolution motors
- 16 microsteps
- Configurable acceleration/deceleration profiles
- SyncDriver for coordinated dual-motor movement

### Obstacle Avoidance

When enabled via `#define EVITEMENT` in [config_robots.hh](lib/config_robots/config_robots.hh:8):
- Three ultrasonic sensors (left, middle, right) monitor surroundings
- Avoidance state machine (`evitement` variable) manages evasion maneuvers:
  - `-1`: Avoidance disabled
  - `0`: Monitoring for obstacles
  - `1-4`: Active avoidance maneuvers (right, left, extended right, extended left)

Per-waypoint avoidance control via `avoidance[]` array allows strategic planning.

## Testing & Calibration

### Odometry Calibration

Calibration process documented in [src/main.cpp](src/main.cpp:196-210):

1. **Rotation calibration**: Uncomment `rotation(360*10)` and adjust `ENTRE_AXE` in config_robots.hh
2. **Straight line calibration**:
   - Uncomment `straight(1000)` to test 1000mm movement
   - Adjust `DIAMETRE_ROUE` (global) and `COEF_DROIT` (per-robot) in config_robots.hh

### Debug Modes

Enable in [config_robots.hh](lib/config_robots/config_robots.hh:6-8):
- `#define TEST_MODE`: Reduces wait time from 85s to 2s for faster testing
- `#define PRINT_DISTANCES`: Prints sensor readings, position, and state to serial
- `#define EVITEMENT`: Enables obstacle avoidance system

## Project Structure

```
lib/
  config_robots/     - Robot configuration (EDIT HERE for waypoints & calibration)
  mouvement/         - Movement primitives and odometry
  stepper_master/    - Low-level stepper motor drivers
  esp32_servo/       - Servo control library
  Superstar/         - Competition-specific actions
  freertos/          - FreeRTOS kernel
src/
  main.cpp          - Main program with dual-core task definitions
platformio.ini      - PlatformIO build configuration
```

## Hardware References

- [Arduino Nano ESP32 Cheat Sheet](https://docs.arduino.cc/tutorials/nano-esp32/cheat-sheet/)
- [Reset Arduino Bootloader](https://support.arduino.cc/hc/en-us/articles/9810414060188-Reset-the-Arduino-bootloader-on-the-Nano-ESP32)

## Competition Timing

- Initial wait: 85 seconds (`GLOBAL_WAIT` in production mode, 2s in TEST_MODE)
- Additional delay: `ADD_DELAY_START` per robot (prevents simultaneous starts)
- Active period: 15 seconds of autonomous navigation
- Auto-stop: Motors disabled at 14.5 seconds, servo celebrates

## Key Global Variables

- `waypointIndex`: Current target in waypoints array
- `equipe`: Team color ('B' = blue, 'J' = yellow)
- `evitement`: Obstacle avoidance state machine
- `Time1`: Match timer start (milliseconds)
- `elapsedTime`: Time since match start

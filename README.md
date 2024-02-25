# Arduino Clock and Timer

This Arduino project allows you to display a clock, set a timer, and manually adjust the time using an LCD display and three buttons.

## Circuit Diagram

![Cool Crift-Kieran](https://github.com/AlpAyyildiz02/Digital-Clock-/assets/161128249/0b82386c-a46f-4aa1-9a1c-01a401946a7f)


## Components Required

- Arduino board (e.g., Arduino Uno)
- 16x2 LCD display with I2C interface
- Pushbuttons (3x)
- Resistors (for pull-up configuration)

## Installation

1. Connect the components according to the circuit diagram.
2. Upload the Arduino code (`clock_timer.ino`) to your Arduino board.
3. Power up the Arduino board.

## Usage

- Press the **MODE** button to cycle through clock mode, timer mode, and set time mode.
- In clock mode, the current time is displayed on the LCD.
- In timer mode, you can start a timer, which will count up from zero.
- In set time mode, you can manually adjust the hours and minutes of the clock using the **ADJUST** button. Use the **LIGHT** button to switch between hours and minutes.

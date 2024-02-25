#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the pins for the buttons
#define MODE_BUTTON_PIN    2
#define ADJUST_BUTTON_PIN  3
#define LIGHT_BUTTON_PIN   4

// Set the LCD address to 0x27 for a 16x2 display
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust the address if necessary

// Enum to represent the different modes
enum Mode {
  CLOCK_MODE,
  TIMER_MODE,
  SET_TIME_MODE
};

Mode currentMode = CLOCK_MODE; // Initial mode is clock mode

bool timerRunning = false; // Flag to indicate whether the timer is running
unsigned long startTime = 0; // Variable to store the start time of the timer
unsigned long lastUpdateTime = 0; // Variable to store the last update time of the clock

int setTimeHours = 0; // Variable to store the hours during time setup
int setTimeMinutes = 0; // Variable to store the minutes during time setup
int setTimeSeconds = 0; // Variable to store the seconds during time setup
bool setTimeToggle = true; // Toggle between hours and minutes during time setup

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight(); // Turn on the backlight

  // Set up button pins
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(LIGHT_BUTTON_PIN, INPUT_PULLUP);

  // Display clock on LCD by default
  displayClock();
}

void loop() {
  // Check if MODE button is pressed
  if (digitalRead(MODE_BUTTON_PIN) == LOW) {
    // Move to the next mode
    currentMode = static_cast<Mode>((currentMode + 1) % 3);
    
    // Clear the LCD
    lcd.clear();
    
    // Display the appropriate mode
    switch (currentMode) {
      case CLOCK_MODE:
        displayClock();
        break;
      case TIMER_MODE:
        startTimer();
        break;
      case SET_TIME_MODE:
        displaySetTime();
        break;
    }
    
    // Wait for button release
    while (digitalRead(MODE_BUTTON_PIN) == LOW) {
      delay(10); // Wait for button debounce
    }
  }
  
  // Handle adjust button in set time mode
  if (currentMode == SET_TIME_MODE && digitalRead(ADJUST_BUTTON_PIN) == LOW) {
    // Increment the time value
    if (setTimeToggle) {
      setTimeHours = (setTimeHours + 1) % 24;
    } else {
      setTimeMinutes = (setTimeMinutes + 1) % 60;
    }
    
    // Display the updated set time
    displaySetTime();
    
    // Wait for button release
    while (digitalRead(ADJUST_BUTTON_PIN) == LOW) {
      delay(10); // Wait for button debounce
    }
  }
  
  // Handle light button in set time mode
  if (currentMode == SET_TIME_MODE && digitalRead(LIGHT_BUTTON_PIN) == LOW) {
    // Toggle between hours and minutes
    setTimeToggle = !setTimeToggle;
    
    // Display the updated set time
    displaySetTime();
    
    // Wait for button release
    while (digitalRead(LIGHT_BUTTON_PIN) == LOW) {
      delay(10); // Wait for button debounce
    }
  }
  
  // Update clock every second
  if (currentMode == CLOCK_MODE) {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdateTime >= 1000) {
      lastUpdateTime = currentTime;
      updateClock();
      displayClock();
    }
  }
  
  // Update timer display if running
  if (currentMode == TIMER_MODE && timerRunning) {
    displayTimer();
    
    // Hold the timer display if the light button is pressed
    while (digitalRead(LIGHT_BUTTON_PIN) == LOW) {
      delay(10); // Wait for button debounce
    }
  }
  
  // Handle adjust button to restart the timer in TIMER_MODE
  if (currentMode == TIMER_MODE && digitalRead(ADJUST_BUTTON_PIN) == LOW) {
    // Restart the timer
    startTimer();
    
    // Wait for button release
    while (digitalRead(ADJUST_BUTTON_PIN) == LOW) {
      delay(10); // Wait for button debounce
    }
  }
}

// Function to start the timer
void startTimer() {
  // Start the timer
  timerRunning = true;
  startTime = millis();
}

// Function to update the clock
void updateClock() {
  // Increment seconds and handle rollover
  setTimeSeconds++;
  if (setTimeSeconds == 60) {
    setTimeSeconds = 0;
    setTimeMinutes++;
    if (setTimeMinutes == 60) {
      setTimeMinutes = 0;
      setTimeHours++;
      if (setTimeHours == 24) {
        setTimeHours = 0;
      }
    }
  }
}

// Function to display the clock on LCD
void displayClock() {
  // Clear the LCD and print the updated time
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Clock: ");
  printDigits(setTimeHours);
  lcd.print(":");
  printDigits(setTimeMinutes);
  lcd.print(":");
  printDigits(setTimeSeconds);
}

// Function to display the set time on LCD
void displaySetTime() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Set Time mode");
  lcd.setCursor(0, 1);
  if (setTimeToggle) {
    // Display hours
    lcd.print("Hours: ");
    lcd.print(setTimeHours);
  } else {
    // Display minutes
    lcd.print("Minutes: ");
    lcd.print(setTimeMinutes);
  }
}

// Function to display the timer on LCD
void displayTimer() {
  // Calculate elapsed time
  unsigned long elapsedTime = millis() - startTime;
  int hours = elapsedTime / 3600000;
  int minutes = (elapsedTime % 3600000) / 60000;
  int seconds = ((elapsedTime % 3600000) % 60000) / 1000;

  // Clear the LCD and print the elapsed time
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Timer: ");
  printDigits(hours);
  lcd.print(":");
  printDigits(minutes);
  lcd.print(":");
  printDigits(seconds);
}

// Function to print two digits with leading zero if needed
void printDigits(int digits) {
  if (digits < 10) {
    lcd.print("0");
  }
  lcd.print(digits);
}

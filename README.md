# DoorLogger 🚪📈

![Platform](https://img.shields.io/badge/platform-Arduino-blue.svg)
![Issues](https://img.shields.io/github/issues/EXELVI/DoorLogger)
![Stars](https://img.shields.io/github/stars/EXELVI/DoorLogger)

**DoorLogger** is a simple and effective door monitoring system built with Arduino. It tracks the number of times a door is opened and closed daily and logs the information to a Discord webhook. The system uses a magnetic switch to detect the door state and an the Arduino UNO R4 integrated RTC (Real-Time Clock) for time management.

## ✨ Features 

- **WiFi Connectivity**: Connects to your WiFi network to send real-time notifications.
- **Discord Integration**: Sends alerts via a Discord webhook when the door is opened or closed.
- **Daily Counter**: Tracks and resets the daily count of door openings.
- **RTC Synchronization**: Synchronizes with an NTP server to keep accurate time.

## 🔧 Components Used 

- **Arduino UNO R4 WIFI**: The main microcontroller board that runs the system. 
- **Magnetic Switch**: Detects the door state (open/closed).

## 🚀 Getting Started 

## 📋 Requirements 

- Arduino IDE installed on your computer
- WiFi network credentials
- Discord webhook URL
- Magnetic switch

## 🛠 Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/EXELVI/DoorLogger.git
   cd DoorLogger
    ```
2. Open the `DoorLogger.ino` file in Arduino IDE.
3. Install the required libraries
4. Add your WiFi credentials and Discord webhook URL in the `arduino_secrets.h` file.
5. Upload the sketch to your Arduino UNO R4 WIFI board.
6. Connect the magnetic switch to the board.

## 📚 Usage
Once the setup is complete and the device is connected to your WiFi network, it will:

1. Monitor the door state using the magnetic switch.
2. Log each open/close event to the console.
3. Send a notification to the configured Discord channel with the event details, including the time and the daily open count.

## 🎲 Example Discord Notification

    ```
    Embed:
    The door was opened!
    - Opened today: <times> times
    - Time: <timestamp>
    ```

## 🤝 Contributing 

Contributions are welcome! Feel free to submit a pull request or open an issue if you have any ideas or improvements.
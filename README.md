# URL Modifier for Chrome

## Overview

This project is designed to enhance browsing experience in Google Chrome by detecting and modifying the URL in the address bar based on specific criteria. It utilizes Windows UI Automation API to monitor Chrome's URL bar for focus changes and employs a global keyboard hook to detect the Enter key press. When the URL bar is focused and the Enter key is pressed, the program modifies the URL based on predefined rules before navigation.

## Features

- Monitors focus changes in Google Chrome to detect when the URL bar is focused.
- Uses a global keyboard hook to detect when the Enter key is pressed.
- Modifies the URL in the address bar based on predefined rules.
- Utilizes Windows UI Automation API for robust interaction with Chrome's UI elements.

## Prerequisites

- Windows operating system.
- Google Chrome browser.
- Visual Studio with C++ development environment set up.
- Basic understanding of COM and Windows UI Automation.


## Usage

1. Run the generated executable. This will start monitoring Google Chrome for URL bar focus changes and key presses.

2. Open Google Chrome and navigate as usual. When the URL bar is focused and the Enter key is pressed, the program will modify the URL based on the predefined rules.

3. To stop the program, simply close the console window or press Ctrl+C in the console.

## Development

This project is structured into several key components:

- `FocusChangedEventHandler`: Monitors focus changes in Google Chrome to detect when the URL bar is focused.
- `KeyboardHook`: Implements a global keyboard hook to detect when the Enter key is pressed.
- `ModifyURL`: Contains the logic for modifying the URL based on predefined rules.
- `Main`: Initializes the components and starts the monitoring process.

You can modify the `ModifyURL` function to change how the URL is modified based on your requirements.

## Contributing

Contributions to this project are welcome! Please fork the repository, make your changes, and submit a pull request.


## Acknowledgments

- Thanks to the Windows UI Automation API for enabling automation tasks on Windows applications.
- This project is for educational purposes and is not affiliated with Google Chrome or Microsoft.

## Disclaimer

This project is intended for educational purposes only. Please use responsibly and at your own risk. The author is not responsible for any potential damages or losses that may arise from using this software.


# XenoOS v0.2.0

## Overview

This is a command-line interface (CLI) system for interacting with various system components such as filesystems, Wi-Fi, SD cards, and system diagnostics. The system supports a variety of commands for managing files, directories, and system configurations.

The system is designed for the entire ESP32 family of microcontrollers but is specifically tailored for the ESP32-C3 model.

## Features

- File management (e.g., create, remove, rename files)
- System diagnostics
- Wi-Fi connectivity management
- SD card interaction
- Command history
- Memory and CPU information

## Command List

This document contains a list of available commands with brief descriptions, parameters, and usage examples.

| Command   | Description                      | Parameters                                      | Example Usage                       |
|-----------|----------------------------------|-------------------------------------------------|-------------------------------------|
| `wget`    | Download files from URL         | `<url> [<path>]`                                | `wget http://example.com/file.txt /path/to/save` |
| `help`    | Display help information        | -                                               | `help`                              |
| `mkdir`   | Create a new directory           | `<path>`                                        | `mkdir /new_directory`             |
| `ls`      | List directory contents         | `<path>`                                        | `ls /sys`                           |
| `cd`      | Change the current directory    | `<path>`                                        | `cd /path/to/directory`            |
| `rm`      | Remove files or directories     | `<path>`                                        | `rm /path/to/file`                 |
| `wifi`    | Manage Wi-Fi connection         | `connect, disconnect, reconnect, scan, status, ip, help` | `wifi connect SSID password`  |
| `free`    | Show available memory           | -                                               | `free`                              |
| `sdcard`  | Manage SD card                  | `info, help`                                    | `sdcard info`                       |
| `clear`   | Clear the screen                | -                                               | `clear`                             |
| `system`  | System commands                 | `restart, info, update, diagnostic, help`       | `system restart`                   |
| `cpu`     | Display processor information   | `info, temp`                                    | `cpu info`                          |
| `cat`     | Read file contents              | `<path_to_file>`                                | `cat /path/to/file`                |
| `rename`  | Rename files                    | `<old_file_path> <new_file_path>`               | `rename /path/oldname /path/newname` |
| `mac`     | Display MAC address             | `info`                                          | `mac info`                          |
| `touch`   | Create an empty file            | `<path_to_file>`                                | `touch /path/to/file`              |
| `mv`      | Move files                      | `<source_path> <destination_path>`              | `mv /path/to/source /path/to/destination` |
| `cp`      | Copy files                      | `<source_path> <destination_path>`              | `cp /path/to/source /path/to/destination` |
| `history` | Command history                 | `clear, read, help`                             | `history clear`                    |
| `hash`    | Calculate hash sum              | `<file_path>`                                   | `hash /path/file`                  |
| `ping`    | Ping a host or IP               | `<host_or_ip>`                                  | `ping 192.168.1.1`                 |
| `find`    | Search for files                | `<path_to_search>`                              | `find /path/to/search`             |
| `df`      | Display filesystem information  | `info`                                          | `df info`                          |
| `tar`     | Work with archives              | `extract, help`                                 | `tar extract archive.tar`          |


## Installation

To install and set up the system, follow these steps:

1. **Prerequisites**:
    - You must have **Arduino IDE 2.x** installed.
    - Ensure the **ESP32 core version 3.1.0 or later** is installed in Arduino IDE.
    - You will also need the following libraries installed:
      - `buildTime` version 1.0.0
      - `ESPping` version 1.0.5
      - `ESP32-targz` version 1.2.6

2. **Hardware Setup**:
    - This system is designed for the **ESP32 family**, specifically developed for the **ESP32-C3**.
    - For the MicroSD card module, use an **SD card with a capacity of no more than 4GB**.
    - The MicroSD connection to the **ESP32-C3** is standard as per the official ESP32-C3 pinout.

3. **Steps to Set Up**:
    1. Clone this repository to your local machine using Git:
    ```bash
    git clone https://github.com/VLPLAY-Games/XenoOS.git
    cd XenoOS
    ```

    2. Open the `XenoOS.ino` file in **Arduino IDE 2**.
    
    3. Select the **ESP32-C3 Dev Module** as your target device in Arduino IDE (or another ESP32 Dev Module family for your board).
    
    4. In the **Tools** tab, configure the following settings:
    - **USB CDC On Boot**: "Enabled"
    - **CPU Frequency**: "160MHz (WiFi)" (default value)
    - **Core Debug Level**: "None" (default value)
    - **Erase All Flash Before Sketch Upload**: "Disabled" (default value)
    - **Flash Frequency**: "80MHz" (default value)
    - **Flash Mode**: "QIO" (default value)
    - **Flash Size**: "4MB (32Mb)" (default value)
    - **JTAG Adapter**: "Disabled" (default value)
    - **Partition Scheme**: "Minimal SPIFFS (1.9MB APP with OTA/190KB SPIFFS)"
    - **Upload Speed**: "921600" (default value)
    - **Zigbee Mode**: "Disabled" (default value)
    
    5. Click **Upload** to flash the system onto your ESP32-C3.

After completing these steps, the system should be running on your device.


## Usage

Once installed, you can interact with the system via Serial port (Speed 115200) in Putty (or Arduino IDE Serial terminal (not recommended))by typing commands directly into the terminal or console. Refer to the "Command List" above for all available commands and their usage examples.

## Diagnostics

To perform a system diagnostic, use the following command:

```bash
system diagnostic
```

This will run a series of tests to check the health of different components such as the ESP32, SPIFFS, SD card, and EEPROM.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

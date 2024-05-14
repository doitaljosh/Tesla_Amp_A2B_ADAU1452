(WIP) Completely custom Arduino code for A2B network bringup in pre-2023 Tesla Model 3/Y vehicles

### Summary:
- Works on STM32 and ESP32 platforms
- Works with either the FOHC mic or amplifier or FOHC mic at nodeadr=1 and amp at nodeadr=2

### Includes:
- Methods for interfacing with an ADAU1452 DSP over SPI or I2C
- Interrupt processing and error handling for A2B discovery
- Initialization functions for the FOHC mic array and premium amplifier

### Todo:
- Support for acting as an A2B I2C slave that spoofs the Tesla amplifier and configures TDM8 output to a DSP
- Add method for storing and loading DSP program memory files from SigmaStudio
- Implement storing settings into non-volatile memory using packed structs
- On ESP32: web interface for controlling DSP runtime parameters
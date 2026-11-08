# STC89C52RC LCD1602 Digital Clock

An embedded digital clock project developed using the **STC89C52RC (8051) microcontroller** and C language.

The project integrates LCD1602 display control, Timer0 interrupt, UART communication, I2C communication, and AT24C02 EEPROM data storage.

It was developed step by step, starting from basic MCU peripheral control and gradually integrating multiple hardware modules and communication protocols.

## Features

- Real-time clock display with AM/PM indication.
- Date display with leap year calculation.
- Time and date adjustment through physical buttons.
- Timer0 interrupt for timekeeping.
- UART communication for transmitting time data to a PC.
- Software I2C implementation for AT24C02 EEPROM communication.
- EEPROM data storage and restoration after power cycling.
- Modular software architecture with separate functional modules.

## Hardware

| Component | Description |
|---|---|
| MCU | STC89C52RC |
| Oscillator | 11.0592 MHz |
| Display | LCD1602 |
| EEPROM | AT24C02 |
| Communication | UART / I2C |
| Development Board | STC89C52RC Development Board |

## Development Environment

- Keil µVision / C51
- STC-ISP
- UART Serial Assistant
- C Language

## Pin Configuration

| Function | MCU Pin |
|---|---|
| LCD RS | P2.6 |
| LCD RW | P2.5 |
| LCD EN | P2.7 |
| LCD Data Bus | P0 |
| I2C SCL | P2.1 |
| I2C SDA | P2.0 |
| Key 1 | P3.1 |
| Key 2 | P3.0 |
| Key 3 | P3.2 |
| UART Trigger Button | P3.3 |

## Software Architecture

The project is organized into independent functional modules to improve code readability, maintainability, and reusability.

| Module | Description |
|---|---|
| `main.c` | Main program flow, peripheral initialization, display updates, and Timer0 interrupt service routine |
| `Timer0.c / .h` | Timer0 initialization and configuration |
| `Time_Setting.c / .h` | Time and date adjustment state machine, leap year calculation, and monthly day calculation |
| `LCD1602.c / .h` | LCD1602 initialization and display functions |
| `Key.c / .h` | Button scanning and software debouncing |
| `UART.c / .h` | UART initialization and time data transmission |
| `I2C.c / .h` | Software I2C implementation |
| `AT24C02.c / .h` | EEPROM read/write operations and time/date data storage |
| `Delay.c / .h` | Software delay function |

## Implementation Details

### 1. Timer0 Interrupt Timekeeping

- Configured Timer0 in Mode 1 (16-bit timer mode).
- Used an 11.0592 MHz oscillator with a 12T clock mode.
- Configured an approximately 1 ms timer interval.
- Accumulated 1000 timer interrupts to generate a one-second time update.
- Implemented second, minute, and hour rollover.
- Implemented AM/PM switching and automatic date updates.

### 2. Time and Date Adjustment

Implemented a state machine to manage the time and date adjustment process.

Supported settings:

- Hour
- Minute
- Second
- AM/PM
- Year
- Month
- Day

Physical buttons are used to increment, decrement, and confirm each setting.

Date calculation follows the Gregorian leap year rules and automatically determines the number of days in each month.

### 3. UART Communication

Configured UART using Timer1 Mode 2 as the baud rate generator.

- Baud Rate: 4800 bps
- Data Format: ASCII
- Transmission Interface: UART

The `UART_SendTime()` function converts the current time into ASCII characters and transmits the data to a PC serial terminal.

Example output:

```text
11:59:53
```

### 4. Software I2C Communication

Implemented I2C communication through GPIO bit-banging.

Implemented basic I2C operations:

- Start Condition
- Stop Condition
- Byte Write
- Byte Read
- Acknowledge (ACK)
- Not Acknowledge (NACK)

The I2C module communicates with the AT24C02 EEPROM for non-volatile data storage.

### 5. AT24C02 EEPROM Data Storage

The system stores time and date information in the AT24C02 EEPROM.

| EEPROM Address | Stored Data |
|---|---|
| 0 | Data Valid Marker |
| 1 | Hour |
| 2 | Minute |
| 3 | Second |
| 4 | AM/PM |
| 5 | Day |
| 6 | Month |
| 7 | Year (Low Byte) |
| 8 | Year (High Byte) |

The year is stored using two bytes and reconstructed when reading data from the EEPROM.

At startup, the system checks the data validity marker and performs basic range validation before restoring the saved time and date.

### 6. EEPROM Data Persistence Verification

Verified EEPROM read/write functionality through the following tests:

1. Write test values to the EEPROM.
2. Read back and verify the stored values.
3. Power off and restart the system.
4. Read the previously stored values again to verify data retention.

This confirms that the time and date settings can be preserved even after power is removed.

## Technical Skills Demonstrated

- **C Programming:** Pointers, bitwise operations, function modularization, conditional statements, and loop control.
- **8051 MCU Peripherals:** GPIO, Timer, Interrupt, and LCD control.
- **Communication Protocols:** UART and I2C.
- **Embedded Software Design:** State machine implementation and software debouncing.
- **Non-volatile Memory:** EEPROM read/write operations and data validation.
- **Debugging:** Keil C51 compilation, debugging, and hardware verification.

## Future Development

This project serves as a foundation for further embedded systems development.

Future learning and development will focus on:

- STM32 microcontroller development.
- ARM Cortex-M architecture.
- RTOS fundamentals and practical implementation.

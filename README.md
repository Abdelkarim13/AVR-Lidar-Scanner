# AVR Ultrasonic Radar / "Lidar" Scanner

A layered AVR firmware project (PlatformIO + Arduino framework) that sweeps an ultrasonic distance sensor with a servo to scan the surrounding area, with LED and buzzer feedback on obstacle detection.

> **Note on naming:** the sensor driven by this firmware (Trigger/Echo pins) is an **ultrasonic (SONAR) sensor**, such as the HC-SR04 — not a true laser LIDAR. The project is referred to as a "Lidar Scanner" for familiarity, but the underlying sensing technology is ultrasonic time-of-flight.

## Overview

The firmware targets an **Arduino Uno (ATmega328P)** and is built with **PlatformIO** using the Arduino framework, while driving peripherals through custom, low-level register access rather than Arduino's high-level APIs. The project is organized in a layered embedded architecture:

- **MCAL** (Microcontroller Abstraction Layer) — direct register-level drivers (GPIO, Timer)
- **HAL** (Hardware Abstraction Layer) — peripheral-specific drivers built on MCAL (Servo, Ultrasonic sensor)
- **APP** (Application Layer) — top-level scan logic tying everything together

Core behavior (inferred from pin roles and available files): a servo sweeps the ultrasonic sensor across an arc, the sensor measures distance to the nearest object at each angle, and the LED/buzzer signal when an obstacle is detected within some threshold. Serial output is configured at 9600 baud, suggesting readings are also logged to the serial monitor (e.g., for plotting or debugging).

## Hardware / Pin Mapping

| Signal        | Port | Pin  | Direction | Purpose                                  |
|---------------|------|------|-----------|-------------------------------------------|
| LED           | D    | PD5  | Output    | Visual indicator (e.g., obstacle alert)   |
| Buzzer        | D    | PD4  | Output    | Audible indicator (e.g., obstacle alert)  |
| Echo          | D    | PD3  | Input     | Ultrasonic sensor echo pulse              |
| Trigger       | D    | PD7  | Output    | Ultrasonic sensor trigger pulse           |
| Servo         | B    | PB1  | Output    | Sweeps the ultrasonic sensor (PWM)        |

> `PB1` on the ATmega328P corresponds to a hardware Timer1 PWM output (`OC1A`), the typical choice for servo control.

## Project Structure

```
.
├── platformio.ini
├── APP/
│   └── *.c              # Application layer — main scan loop, ties Servo + Ultrasonic + alerts together
├── HAL/
│   ├── Servo/            # Servo sweep control, built on MCAL/Timer PWM
│   └── Ultrasonic/       # Trigger/Echo timing and distance calculation
└── MCAL/
    ├── GPIO/
    │   ├── gpio.h         # Port/pin/state definitions and function prototypes
    │   └── gpio.c         # Low-level digital I/O driver (this file)
    └── Timer/             # Timer driver (PWM generation / pulse timing)
```

> `APP`, `HAL/Servo`, `HAL/Ultrasonic`, and `MCAL/Timer` sources were not included when this README was generated — their descriptions above are inferred from `gpio.c`'s pin comments and `platformio.ini`'s build filters. Share those files if you'd like this section made exact.

## Requirements

- [PlatformIO](https://platformio.org/) (CLI or IDE extension for VS Code)
- Arduino Uno board (or ATmega328P-based equivalent)
- USB cable for programming and serial monitoring
- HC-SR04 (or compatible) ultrasonic sensor
- Standard hobby servo (e.g., SG90)

## Building

```bash
pio run
```

## Uploading

```bash
pio run --target upload
```

## Monitoring Serial Output

```bash
pio device monitor
```

This connects at **9600 baud** as configured in `platformio.ini`.

## Wiring Summary

- Ultrasonic **Trig** → **PD7**, **Echo** → **PD3**
- **Servo** signal → **PB1**
- **LED** (with current-limiting resistor) → **PD5**
- **Buzzer** → **PD4**
- Power the servo and ultrasonic sensor per their datasheet requirements — many need a separate 5V supply capable of more current than a single MCU pin can safely source
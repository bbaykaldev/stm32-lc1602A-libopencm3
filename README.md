# STM32 + 1602A LCD Tutorial (libopencm3, 4-bit Mode)

This project shows how to control a 1602A LCD in 4-bit mode using an STM32 Blue Pill (STM32F103C8T6) and the libopencm3 low-level C library. The C file contains a full working example code.

---

## 🔌 Wiring

| LCD Pin | Connect To (STM32) |
|---------|--------------------|
| RS      | PA1                |
| E       | PA3                |
| D4      | PB0                |
| D5      | PB1                |
| D6      | PB10               |
| D7      | PB11               |
| RW      | GND                |
| VSS     | GND                |
| VDD     | 5V (or 3.3V)       |
| VO      | GND (or via pot)   |
| A       | 3.3V or 5V         |
| K       | GND                |

Make sure the **VO pin (contrast)** is either grounded or connected to a potentiometer for proper visibility.

---

## ⚙️ Building the Code

This project uses **PlatformIO** and **libopencm3**.

### Prerequisites:
- [PlatformIO Core or PlatformIO IDE](https://platformio.org/)
- A Blue Pill STM32F103C8T6 board
- ST-Link or USB-UART programmer

### To Build and Upload:

```bash
pio run
pio upload

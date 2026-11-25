# Wiring Guide

See the wiring tables in `README.md`.

## MFRC522 ↔ Arduino UNO

```text
MFRC522       Arduino UNO
SDA / SS  --> D10
SCK       --> D13
MOSI      --> D11
MISO      --> D12
RST       --> D9
3.3V      --> 3.3V
GND       --> GND
```

## Indicators and servo

```text
Servo signal        --> D6
Red LED anode       --> D4 through 220 ohm resistor
Green LED anode     --> D5 through 220 ohm resistor
Buzzer signal       --> D3
All grounds         --> Arduino GND
```

MFRC522 must be powered at 3.3V. For a servo, use a separate regulated supply if needed and join its ground to Arduino GND.

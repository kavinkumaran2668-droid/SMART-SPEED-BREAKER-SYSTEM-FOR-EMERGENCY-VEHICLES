# Smart Speed Breaker System for Emergency Vehicles

An Arduino-based educational prototype that uses an RFID tag to identify an authorized emergency vehicle. When the registered tag is scanned, a servo motor moves the model speed breaker to its lowered position for a fixed clearance period. It then returns to its normal raised position.

## Features

- RFID-based emergency vehicle identification
- Servo-controlled model speed breaker
- Red LED for normal state and green LED for emergency clearance
- Buzzer indication
- Automatic return to the normal position after the clearance timer
- Serial Monitor output for debugging and RFID UID registration

## Hardware required

| Component | Quantity |
|---|---:|
| Arduino UNO or Nano | 1 |
| MFRC522 RFID reader | 1 |
| RFID card/tag (one for authorized testing) | 1 |
| SG90 servo motor (for a lightweight model only) | 1 |
| Red LED | 1 |
| Green LED | 1 |
| 220 ohm resistors | 2 |
| Active/passive buzzer | 1 |
| Breadboard and jumper wires | As required |
| Regulated 5V supply for servo, if needed | 1 |

## Wiring

### MFRC522 to Arduino UNO

| MFRC522 pin | Arduino UNO pin |
|---|---|
| SDA / SS | D10 |
| SCK | D13 |
| MOSI | D11 |
| MISO | D12 |
| RST | D9 |
| 3.3V | 3.3V |
| GND | GND |

**Important:** MFRC522 is a 3.3V device. Connect its power pin to 3.3V, not 5V. Use suitable 3.3V logic-level interfacing if required by your board/module.

### Other components

| Component | Arduino pin |
|---|---|
| Servo signal | D6 |
| Red LED anode (through 220Ω resistor) | D4 |
| Green LED anode (through 220Ω resistor) | D5 |
| Buzzer signal | D3 |
| LED cathodes / buzzer GND | GND |
| Servo supply | Regulated 5V (see note below) |

All grounds must be connected together. Do not power a servo from the Arduino 5V pin if the servo draws more current than the board can safely supply. Use a separate regulated 5V supply and connect its GND to Arduino GND.

## Arduino IDE setup

1. Install the **Arduino IDE**.
2. In Library Manager, install **MFRC522** (Miguel Balboa) and **Servo** (usually included with the IDE).
3. Open `smart_speed_breaker.ino`.
4. Select **Arduino UNO** (or your actual board) and the correct COM port.
5. Upload the sketch.
6. Open Serial Monitor at **9600 baud**.

## Register your emergency RFID card

The sketch contains an example UID:

```cpp
const byte AUTHORIZED_UID[] = {0xDE, 0xAD, 0xBE, 0xEF};
```

This is only a placeholder. To register your own card:

1. Upload the sketch as it is.
2. Open Serial Monitor at 9600 baud.
3. Scan the RFID card/tag you want to authorize.
4. Copy the UID printed as hexadecimal bytes, for example `A1 B2 C3 D4`.
5. Replace the placeholder with your UID:

```cpp
const byte AUTHORIZED_UID[] = {0xA1, 0xB2, 0xC3, 0xD4};
```

6. Upload the sketch again.

The UID length must match the card's UID length. This simple UID check is suitable for a classroom demonstration, not for secure access control.

## Adjusting the model

In the sketch, change these values to match your mechanical model:

```cpp
const int RAISED_ANGLE  = 90;
const int LOWERED_ANGLE = 0;
const unsigned long CLEARANCE_TIME_MS = 10000UL;
```

- `RAISED_ANGLE`: servo position for the normal raised breaker.
- `LOWERED_ANGLE`: servo position for the lowered/flat model.
- `CLEARANCE_TIME_MS`: how long the breaker remains lowered (10,000 ms = 10 seconds).

Test servo angles without a physical load first. The actual mechanism must move freely and must not trap or strike anything.

## How it works

1. The system starts with the model speed breaker in the raised position.
2. The RFID reader scans a vehicle tag.
3. The Arduino compares the scanned UID with the registered emergency UID.
4. If the UID matches, the green LED turns on, the buzzer beeps and the servo lowers the model breaker.
5. After the clearance period, the servo returns the breaker to the raised position and the red LED turns on.
6. An unknown tag does not activate the system.

## Repository structure

```text
smart-speed-breaker/
├── smart_speed_breaker.ino
├── README.md
├── wiring.md
├── LICENSE
└── .gitignore
```

## Safety and project limitations

This is a **low-voltage classroom prototype**, not a road-ready traffic system. RFID only confirms that a registered tag is nearby; it does not prove that an emergency vehicle has safely passed. A real road installation must not raise a barrier while a vehicle is crossing and would require independent vehicle-presence sensing, fail-safe mechanical design, manual override, backup power, cybersecurity, testing, and approvals from traffic/road authorities. Never test this prototype on a public road or with a full-size vehicle.

## Future improvements

- Add an independent vehicle-presence sensor to confirm the road is clear before returning the breaker.
- Add wireless monitoring and event logging.
- Add multiple authorized RFID IDs.
- Add a manual emergency override and fault detection.

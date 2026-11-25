/*
  Smart Speed Breaker System for Emergency Vehicles
  Board: Arduino UNO / Nano (5V logic)
  Modules: MFRC522 RFID reader, servo motor, red/green LEDs, buzzer

  Behaviour:
  - Normal state: speed breaker servo is in RAISED position.
  - When a registered emergency-vehicle RFID card/tag is detected:
      * buzzer sounds briefly
      * green LED turns on
      * servo moves to LOWERED position for the configured time
  - After the clearance period, the breaker returns to RAISED position.

  IMPORTANT:
  This is an educational tabletop prototype. Do not connect it to a real
  road mechanism. A real installation needs certified fail-safe controls,
  vehicle detection redundancy, safety interlocks, weatherproofing and
  approval from the relevant road/traffic authorities.
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

// ---------- Pin configuration ----------
#define RFID_SS_PIN   10
#define RFID_RST_PIN  9
#define SERVO_PIN     6
#define RED_LED_PIN   4
#define GREEN_LED_PIN 5
#define BUZZER_PIN    3

MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN);
Servo breakerServo;

// ---------- Mechanical positions (adjust for your model) ----------
const int RAISED_ANGLE  = 90;  // Speed breaker raised
const int LOWERED_ANGLE = 0;   // Speed breaker lowered

// Time the breaker stays lowered after a valid emergency tag.
const unsigned long CLEARANCE_TIME_MS = 10000UL;

// Replace this example UID with your emergency vehicle card/tag UID.
// UID is printed to Serial Monitor when an unknown tag is scanned.
// Example format: {0xDE, 0xAD, 0xBE, 0xEF}
const byte AUTHORIZED_UID[] = {0xDE, 0xAD, 0xBE, 0xEF};
const byte AUTHORIZED_UID_LENGTH = sizeof(AUTHORIZED_UID);

bool isLowered = false;
unsigned long loweredAt = 0;

bool isAuthorizedCard() {
  if (rfid.uid.size != AUTHORIZED_UID_LENGTH) {
    return false;
  }

  for (byte i = 0; i < AUTHORIZED_UID_LENGTH; i++) {
    if (rfid.uid.uidByte[i] != AUTHORIZED_UID[i]) {
      return false;
    }
  }
  return true;
}

void printScannedUID() {
  Serial.print(F("Scanned UID: "));
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) Serial.print('0');
    Serial.print(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) Serial.print(' ');
  }
  Serial.println();
}

void lowerBreaker() {
  Serial.println(F("Authorized emergency vehicle detected."));
  Serial.println(F("Lowering speed breaker..."));

  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  tone(BUZZER_PIN, 1500, 300);

  breakerServo.write(LOWERED_ANGLE);
  isLowered = true;
  loweredAt = millis();
}

void raiseBreaker() {
  Serial.println(F("Clearance time completed. Raising speed breaker..."));

  breakerServo.write(RAISED_ANGLE);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);

  isLowered = false;
}

void setup() {
  Serial.begin(9600);
  while (!Serial) { ; }

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);

  breakerServo.attach(SERVO_PIN);
  breakerServo.write(RAISED_ANGLE);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println(F("------------------------------------"));
  Serial.println(F(" Smart Speed Breaker System Ready"));
  Serial.println(F(" Scan an emergency vehicle RFID tag"));
  Serial.println(F("------------------------------------"));
}

void loop() {
  // Return the breaker to its normal raised position after the timer.
  if (isLowered && (millis() - loweredAt >= CLEARANCE_TIME_MS)) {
    raiseBreaker();
  }

  // Check whether a new RFID card/tag is present.
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  printScannedUID();

  if (isAuthorizedCard()) {
    // Refresh the clearance period if the authorized tag is scanned again.
    if (!isLowered) {
      lowerBreaker();
    } else {
      loweredAt = millis();
      Serial.println(F("Emergency tag scanned again; clearance timer extended."));
      tone(BUZZER_PIN, 1500, 150);
    }
  } else {
    Serial.println(F("Unauthorized tag. Breaker remains raised."));
    tone(BUZZER_PIN, 500, 150);
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

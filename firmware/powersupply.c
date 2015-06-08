#include <avr/io.h>
#include <stdio.h>   // Benutzung von nicht ANSI-C Funktion
#include <stdlib.h>  // Benutzung von nicht ANSI-C Funktion
#include <stdbool.h> // Benutzung von BOOL Variablen
#include <twislave.h>
#include "powersupply.h"
#include "lcd-routines.h"
#include "rotary-encoder.c"

double readVoltage;         // Spannungs Istwert
double readVoltageOld = -2; //
double readCurrent;         // Strom Istwert
double readCurrentOld = -2; //
double setVoltage = 2;      // Spannungs Sollwert
double setVoltageOld = -2;  //
double setCurrent = 2;      // Strom Istwert
double setCurrentOld = -2;  //
uint8_t dutyCycle = 150;    // Tastgrad PWM
uint8_t dutyCycleOld = 0;
int8_t choice = 0;             // Menüauswahl
int8_t choiceOld = -2;         //
int8_t inputChoice = 0;        // Eingangsauswahl
int8_t inputChoiceOld = -2;    //
bool menuActive = true;        // Flag für Menü
char Buffer[20];               // Buffer für LCD-String
bool i2cIsInitialized = false; // Flag fuer I2C

// Initialisierung von SPI Master Modus
void SPI_Init_Master(void) {
  // Setze MOSI und SCK als Ausgang
  DDRB |= (1 << PB5) | (1 << PB4) | (1 << PB7);
  PORTB &= ~((1 << PB5) | (1 << PB7));
  PORTB |= (1 << PB4);

  // Aktiviere SPI und setze als Master
  // Setze Vorteiler auf 16 und aktiviere Interrupts
  SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << SPR1);
}

// Funktion zum Senden von Daten
void SPI_Tranceiver(unsigned char data) {
  // Lade Daten in Buffer
  SPDR = data;

  // Warte bis zum Ende der Übertragung
  while (!(SPSR & (1 << SPIF)))
    ;
}

// Initialisierung von Analog Digital Konverter
void ADC_Init(void) {
  DDRA &= 0x7F;
  ADMUX = (1 << REFS0);
  ADCSRA = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  ADCSRA |= (1 << ADEN);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;
  (void)ADCW;
}

// Funktion zum Auslesen der ADC Kanäle
uint16_t ADC_Read(uint8_t channel) {
  ADMUX = (ADMUX & ~(0x1f)) | (channel & 0x1F);
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC)) {
  }
  return ADCW;
}

// Initialisierung vom PWM
void PWM_Init(void) {
  DDRD = 0x20; // Setze Port D Pin 5
  // Fast-PWM Invertiert 8 Bit
  TCCR1A = (1 << WGM10) | (1 << COM1A0) | (1 << COM1A1);
  TCCR1B = (1 << WGM12) | (1 << CS10); // Setze Vorteiler auf 1024
  OCR1A = 200;
}

// Funktion zur Übertragung der Sollspannung
void Voltage_Tranceiver(float voltage) {
  if (voltage < 2)
    voltage = 2;
  uint16_t temp = (voltage)*204.75; // Spannung 20 Volt auf 12 Bit
  SPI_Tranceiver(temp >> 8);        // Übertragung MSB
  SPI_Tranceiver(temp);             // Übertragung LSB
  PORTB &= ~(1 << PB4);             // Daten laden
  PORTB |= (1 << PB4);
}

// Funktion zum Einstellen der Sollspannung
void Voltage(void) {
  if (!menuActive) // Setze Spannung nur wenn Menü nicht ausgewählt
    setVoltage += encode_read() * 0.05; // 0,001 Volt pro Schritt
  if (setVoltage > 20)                  // Spannungsgrenze
    setVoltage = 20;                    // maximal 20 Volt
  if (setVoltage < 2)                   // Spannungsgrenze
    setVoltage = 2;                     // minimal 2 Volt
  if (setVoltage != setVoltageOld) {
    lcd_setcursor(0, 2);               // Zeigerposition
    dtostrf(setVoltage, 1, 1, Buffer); // Dezimal zu String
    lcd_string(Buffer);                // Ausgabe von String
    lcd_string("V    ");               // Einheit und löschen von Artefakten
    setVoltageOld = setVoltage;
  }
}

// Funktion zum setzen der Strombegrenzung
void Current(void) {
  if (!menuActive) // Setze Strom nur wenn Menü nicht ausgewählt
    setCurrent += encode_read() * 0.005; // 0,001 Ampere pro Schritt
  if (setCurrent > 2)                    // Stromgrenze
    setCurrent = 2;                      // maximal 2 Ampere
  if (setCurrent < 0.01)                 // Stromgrenze
    setCurrent = 0.01;                   // minimal 0.01 Ampere
  if (setCurrent != setCurrentOld) {
    lcd_setcursor(0, 2);               // Zeigerposition
    dtostrf(setCurrent, 1, 2, Buffer); // Dezimal zu String
    lcd_string(Buffer);                // Ausgabe von String
    lcd_string("A    ");               // Einheit und löschen von Artefakten
    setCurrentOld = setCurrent;
  }
}

// Funktion zum Setzen der Spannung über Port C
void Port_Input() {
  if (0x00 != DDRC) { // Wenn Port C noch nicht als Eingang,
    DDRC = 0x00;      // dann als Eingang setzen
  }
  setVoltage = PORTC * 0.078431373;
}

// Funktion zum Auslesen der Spannung über Port C
void Port_Output() {
  if (0xff != DDRC) { // Wenn Port C noch nicht als Ausgang,
    DDRC = 0xff;      // dann als Eingang setzen
  }
  PORTC = (ADC_Read(6) >> 2); // Istspannung an Port C ausgeben
}

// Funktion zum Setzen von Sollspannung oder Sollstrom
void USB_Set(uint8_t valueChoice, uint8_t value) {
  if (0 == inputChoice) {       // Wenn Eingangswahl auf USB,
    switch (valueChoice) {      // dann Spannung oder Strom setzen
    case 0:                     // Setzen der Spannung
      setVoltage = value * 0.1; // Runtersetzen um eine Zehnerpotenz
      break;
    case 1:                      // Setzen der Strombegrenzung
      setCurrent = value * 0.01; // Runtersetzen um zwei Zehnerpotenz
      break;
    default:
      break;
    }
  }
}

// Funktion zum Auslesen der Istspannung und Iststrom
void USB_Read(uint8_t *voltage, uint8_t *current) {
  *voltage = readVoltage * 10;  // Raufsetzen um eine Zehnerpotenz
  *current = readCurrent * 100; // Raufsetzen um zwei Zehnerpotenz
}

// Funktion zum Setzen des LCD Kontrast
void Contrast(void) {
  if (!menuActive) // Setze Kontrast nur wenn Menü nicht ausgewählt
    dutyCycle += encode_read(); // Tastgrad mit Drehimpulsgeber einstellen
  if (dutyCycle > 200)          // Tastgradgrenze
    dutyCycle = 200;            // maximal 200
  if (dutyCycle < 2)            // Tastgradgrenze
    dutyCycle = 2;              // minimal 2
  if (dutyCycleOld != dutyCycle) {
    lcd_setcursor(0, 2);              // Zeigerposition
    itoa(dutyCycle >> 1, Buffer, 10); // Tastgrad halbieren und in String
    lcd_string(Buffer);               // Tastgrad in Prozent
    lcd_string("%     ");             // Einheit
    OCR1A = dutyCycle + 50;           // Tastgrad addiert mit 50 setzen
    dutyCycleOld = dutyCycle;
  }
}

// Funktion zum wählen des Eingangs
void Input(void) {
  if (!menuActive) // Setze Eingang nur wenn Menü nicht ausgewählt
    inputChoice += encode_read();      // Drehimpulsgeber +2,-2
  if (inputChoice > 8)                 // Eingangsgrenze
    inputChoice = 8;                   // maximal 8 (4)
  if (inputChoice < 0)                 // Eingangsgrenze
    inputChoice = 0;                   // minimal 0
  if (inputChoice != inputChoiceOld) { // Überprüfe änderung der Wahl
    inputChoiceOld = inputChoice;      // Wahl zwischenspeichern
    switch (inputChoice >> 1) {        // Wähle Eingangswahl
    case 0x00:                         // USB als Eingang
      lcd_setcursor(0, 2);
      lcd_string("USB        ");
      break;
      if (i2cIsInitialized == true) {
        close_twi_slave();
        i2cIsInitialized = false;
      }
    case 0x01: // Port als Eingang
      lcd_setcursor(0, 2);
      lcd_string("Port in    ");
      Port_Input(); // Einstellen der Sollspannung
      if (i2cIsInitialized == true) {
        close_twi_slave();
        i2cIsInitialized = false;
      }
      break;
    case 0x02: // Port als Ausgang
      lcd_setcursor(0, 2);
      lcd_string("Port out   ");
      Port_Output(); // Ausgabe der Istspannung
      if (i2cIsInitialized == true) {
        close_twi_slave();
        i2cIsInitialized = false;
      }
      break;
    case 0x03: // Einstellen mit Drehimpulsgeber
      lcd_setcursor(0, 2);
      lcd_string("Manuell    ");
      break;
      if (i2cIsInitialized == true) {
        close_twi_slave();
        i2cIsInitialized = false;
      }
    case 0x04: // Einstellen mit I2C
      lcd_setcursor(0, 2);
      lcd_string("I2C        ");
      if (i2cIsInitialized == false) {
        init_twi_slave(0x50);
        i2cIsInitialized = true;
      }
    default:
      break;
    }
  }
}

// Funktion zum Überwachen der Strombegrenzung
void Max_Current(void) {
  if (readCurrent > setCurrent) { // Istwert mit Maximalwert vergleichen
    setVoltage = (readVoltage / readCurrent) * setCurrent; // Spannung berechnen
    i2cdata[0] = (uint8_t)(setVoltage * 10);
  }
  Voltage_Tranceiver(setVoltage); // Spannung übertragen
}

// Funktion zum Einlesen und zum Anzeigen der Istwerte
void Refresh(void) {
  lcd_setcursor(11, 1);
  readVoltage = (ADC_Read(6) >> 2) * 0.1; // 10 Bit auf 8 Bit
  if (readVoltage > readVoltageOld + 0.1 ||
      readVoltage < readVoltageOld - 0.1) {
    dtostrf(readVoltage, 1, 1, Buffer); // Dezimal zu String
    lcd_string(Buffer);
    lcd_string("V     ");
    i2cdata[2] = readVoltage * 10;
    readVoltageOld = readVoltage;
  }
  lcd_setcursor(11, 2);
  readCurrent = (ADC_Read(7) >> 2) * 0.01; // 10 Bit auf 8 Bit
  if (readCurrent > readCurrentOld + 0.05 ||
      readCurrent < readVoltageOld - 0.05) {
    dtostrf(readCurrent, 1, 2, Buffer); // Dezimal zu String
    lcd_string(Buffer);
    lcd_string("A     ");
    i2cdata[3] = readCurrent * 100;
    readCurrentOld = readCurrent;
  }
  // I2C Routine
  if (inputChoice >> 1 == 0x04) {
    setVoltage = (uint8_t)(i2cdata[0] * 0.1);
    setCurrent = (uint8_t)(i2cdata[1] * 0.01);
  }
}

// Funktion zum Initialisieren aller Module
void PSU_Init(void) {
  lcd_init();            // Initialisierung des LCD
  lcd_home();            // Zeigerposition
  lcd_string("Init..."); // Status ausgeben
  SPI_Init_Master();     // Initialisierung des SPI
  ADC_Init();            // Initialisierung des A/D Wandler
  PWM_Init();            // Initialisierung der PWM
  encode_init();         // Initialisierung des Drehimpulsgeber
  lcd_clear();           // LCD Anzeige löschen
  Voltage_Tranceiver(2); // Spannung auf 2 Volt einstellen
}

// Funktion für die Navigation durchs Menü
void Menu(void) {
  if (button()) // Menü Flag toggeln
    menuActive = !menuActive;
  if (menuActive) {            // Menü bei gesetztem Flag aufrufen
    choice += encode_read();   // Auswahl mit Drehimpulsgeber einstellen
    if (choice > 6)            // Auswahlgrenze
      choice = 6;              // maximal 6
    if (choice < 0)            // Auswahlgrenze
      choice = 0;              // minimal 0
    if (choice != choiceOld) { // Bei geänderter Auswahl
      choiceOld = choice;      // Vorherigen Wert speichern
      switch (choice >> 1) {   // Auswahl halbieren
      case 0x00:
        lcd_home();
        lcd_string("Spannung  "); // Spannung einstellen
        setVoltageOld++;
        break;
      case 0x01:
        lcd_home();
        lcd_string("Max. Strom"); // Strombegrenzung einstellen
        setCurrentOld++;
        break;
      case 0x02:
        lcd_home();
        lcd_string("Eingang   "); // Eingang auswählen
        inputChoiceOld++;
        break;
      case 0x03:
        lcd_home();
        lcd_string("Kontrast  "); // Kontrast einstellen
        dutyCycleOld++;
        break;
      default:
        break;
      }
    }
  }
  switch (choice >> 1) {
  case 0x00:
    Voltage(); // Funktion zur Spannungseinstellung aufrufen
    break;
  case 0x01:
    Current(); // Funktion zur Strombegrenzung aufrufen
    break;
  case 0x02:
    Input(); // Funktion zur Eingangsauswahl aufrufen
    break;
  case 0x03:
    Contrast(); // Funktion zum Kontrasteinstellung aufrufen
    break;
  default:
    break;
  }
}

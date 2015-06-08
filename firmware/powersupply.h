#ifndef POWERSUPPLY_H
#define POWERSUPPLY_H
#define XTAL 16e6 // 16MHz

#define PHASE_A (PINB & 1 << PB3)
#define PHASE_B (PINB & 1 << PB2)
#define TASTERPORT PINB
#define TASTERBIT PINB1

void PSU_Init(void);
void Menu(void);
void Refresh(void);
void Max_Current(void);
void USB_Set(uint8_t valueChoice, uint8_t value);
void USB_Read(uint8_t *voltage, uint8_t *current);
#endif

/*
 * WS_LCD.h
 *
 * Created: 1/21/2015 4:30:54 PM
 *  Author: dev-box
 */ 

#include <stddef.h>
#include <avr/io.h>


#ifndef WS_LCD_H_
#define WS_LCD_H_



void twiInit(void);
void twiStart(void);
void twiStop(void);
void twiSend(uint8_t u8data);
void twiSendCmd(uint8_t fillData);

void lcdInit(void);
void lcdSendCommand(uint8_t fillData);
void lcdSetPos(uint8_t row, uint8_t column);
void lcdFill(uint8_t fillData);
void lcdInvert(uint8_t data);
void lcdWriteChar(uint8_t data);
void lcdPrint(const char data[18]);

#endif /* WS_LCD_H_ */
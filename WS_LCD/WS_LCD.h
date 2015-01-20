#include <stddef.h>
#include <avr/io.h>

#ifndef __INCLUDE_WS_LCD__
#define __INCLUDE_WS_LCD__

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

#endif __INCLUDE_WS_LCD__
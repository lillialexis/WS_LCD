/*
 * WS_LCD.h
 *
 * Created: 1/21/2015 4:30:54 PM
 *  Author: dev-box
 */ 

#include <stddef.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#ifndef WS_LCD_H_
#define WS_LCD_H_


#ifndef BOOL
#define BOOL uint8_t
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

//void twiInit(void);
//void twiStart(void);
//void twiStop(void);
//void twiSend(uint8_t u8data);
//void twiSendCmd(uint8_t fillData);

//#define MEMORY_SAVING

//typedef enum {
	//FONT_SIZE_SMALL = 0,
	//FONT_SIZE_MEDIUM,
	//FONT_SIZE_LARGE,
	//FONT_SIZE_XLARGE
//} FONT_SIZE;
//
//#define FLAG_PAD_ZERO 1
//#define FLAG_PIXEL_DOUBLE_H 2
//#define FLAG_PIXEL_DOUBLE_V 4
//#define FLAG_PIXEL_DOUBLE (FLAG_PIXEL_DOUBLE_H | FLAG_PIXEL_DOUBLE_V)

	void lcdInit(void);

	//void lcdSetFontSize(FONT_SIZE size);
	//void lcdSetFlags(uint8_t flags);
	void lcdSetContrast(uint8_t contrast);

	//FONT_SIZE lcdGetFontSize();
	//uint8_t lcdGetFlags();
	uint8_t lcdGetContrast();
	
	uint8_t lcdGetMaxRows();
	uint8_t lcdGetMaxCols();
	
	uint8_t lcdGetCurrentRow();
	uint8_t lcdGetCurrentCol();

	void lcdSetPos(uint8_t row, uint8_t column);
		
	void lcdSetInvert(BOOL invert);
	BOOL lcdGetInvert();
		
	void lcdFill(char fillData);
	
	void lcdClearLine(uint8_t line);
	void lcdClearScreen();	

	void lcdReplaceLine(uint8_t line, char *buffer);
	void lcdPrintln(char *buffer);
	void lcdPrint(char *buffer);

	void lcdReplaceLine_P(uint8_t line, const PROGMEM char *buffer);
	void lcdPrintln_P(const PROGMEM char *buffer);
	void lcdPrint_P(const PROGMEM char *buffer);

	void lcdDraw(const PROGMEM uint8_t* buffer, uint8_t width, uint8_t height);
	 
	//void lcdStartScrollRight(uint8_t start, uint8_t stop);
	//void lcdStartScrollLeft(uint8_t start, uint8_t stop);
	 //
	//void lcdStartScrollDiagRight(uint8_t start, uint8_t stop);
	//void lcdStartScrollDiagLeft(uint8_t start, uint8_t stop);
	//void lcdStopScroll(void);
	int a2i(char *s);

#endif /* WS_LCD_H_ */
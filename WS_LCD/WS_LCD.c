/*
 * WS_LCD.cpp
 *
 * Created: 1/20/2015 1:28:07 PM
 *  Author: Jesse
 */

#include "WS_LCD.h"
#include "LCDConstants.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stddef.h>
#include <string.h>

uint8_t   lcdRow      = 0;
uint8_t   lcdColumn   = 0;
//FONT_SIZE lcdFont     = 0;
//uint8_t   lcdFlags    = 0;
uint8_t   lcdContrast = CONTRAST_LEVEL; // was 0xcf in SSD103.cpp
BOOL      lcdInvert   = OLED_NORMAL;

void twiInit(void)
{
	TWSR = 0x00; //clears the prescaler (twps0 twps1) bits for F set
	TWBR = 0x0c; //set SCL to 400kHz
	TWCR = (1<<TWINT) | (1<<TWEN); //enable TWI should configure pins, reset flags
}

void twiStart(void){	
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);	//send start condition 
	while ((TWCR & (1<<TWINT)) == 0); //wait for start condition to ack
}

void twiStop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO); //send stop condition
	for(uint8_t ct = 0; ct < 40; ct++); //wait a moment in case a start immediately follows
}

void twiSend(uint8_t u8data)
{
	TWDR = u8data; //data to dat register
	TWCR = (1<<TWINT)|(1<<TWEN); //set TWI byte ready to go
	while ((TWCR & (1<<TWINT)) == 0); //wait for data to send
}

void twiSendCmd(uint8_t command)
{
	twiSend(COMMAND_COMMAND);   // command command
	twiSend(command);// command value
}

void lcdSendCommand(uint8_t command)
{
	twiStart();
	twiSend(OLED_ADDRESS);
	twiSendCmd(command);
	twiStop();
}

void lcdInit(void)
{ 	
	twiInit();
	
	//*********** 96x16 / 128x64 ****************
	twiStart();
	twiSend(OLED_ADDRESS);
	twiSendCmd(OLED_SLEEP); // 0xae display off
	twiSendCmd(0xd5);       // set display clock divide ratio
		twiSendCmd(0x80);   // set ratio 128x64:80 96x16:f0
	twiSendCmd(0xa8);       // set multiplex ratio (screen lines 1 to 64)
		twiSendCmd(MULTIPLEX_RATIO); // set 16:0x0f 64:0x3f
	twiSendCmd(0xd3);       // set display offset
		twiSendCmd(0);      // not offset
	twiSendCmd(DISPLAY_START_LINE); // set display start line
	twiSendCmd(0x8d);       // charge pump control
		twiSendCmd(0x14);   // 0x14:Run 0x10:off
	twiSendCmd(0xd8);       // set display mode
		twiSendCmd(0x05);   // low power
	twiSendCmd(SEGMENT_REMAP); // --set segment re-map 96 to 1
	twiSendCmd(0xC8);       // --Set COM Output Scan Direction 16 to 1
	twiSendCmd(0xda);       // --set com pins hardware configuration
		twiSendCmd(COM_PIN_RATIO); // set ratio 128x64:0x12 96x16:0x02
	twiSendCmd(0x81);       // --set contrast control register
		twiSendCmd(CONTRAST_LEVEL);
	twiSendCmd(0xd9);       // --set pre-charge period
		twiSendCmd(0xf1);   // set ratio 128x64:0xf1 96x16:0x22
	twiSendCmd(0xdb);       // --set vcomh
		twiSendCmd(VCOM_DETECT); // --0.77vref
	twiSendCmd(0x20);       // Set Memory Addressing Mode
		twiSendCmd(0x00);   // 00, Horizontal Addressing Mode; 01, Vertical Addressing Mode; 10, Page Addressing Mode (RESET); 11, Invalid
	twiSendCmd(OLED_RUN);
	twiSendCmd(OLED_NORMAL);
	twiSendCmd(OLED_ACTIVE); // --turn on oled panel
	
	twiStop();
}

//void lcdSetFontSize(FONT_SIZE size) { lcdfont = size; }
//void lcdSetFlags(uint8_t flags)     { lcdflags = flags; }

//FONT_SIZE lcdGetFontSize() { return lcdFont; }
//uint8_t   lcdGetFlags()    { return lcdFlags; }

void lcdSetContrast(uint8_t contrast) 
{	
	lcdContrast = contrast; 
	twiSendCmd(0x81); // --set contrast control register
	twiSendCmd(lcdContrast);
}
uint8_t   lcdGetContrast() { return lcdContrast; }

uint8_t   lcdGetMaxRows()  { return MAX_ROWS; }
uint8_t   lcdGetMaxCols()  { return MAX_COLS; }

uint8_t   lcdGetCurrentRow()  { return lcdRow; }
uint8_t   lcdGetCurrentCol()  { return lcdColumn; }

void lcdSetPos(uint8_t row, uint8_t column)
{
	twiStart();
	twiSend(OLED_ADDRESS);
	twiSendCmd(0xb0+row);
	twiSendCmd(((column&0xf0)>>4) | HIGH_COL_START_ADDR); // high column start address or'd 0x10
	twiSendCmd((column&0x0f) | LOW_COL_START_ADDR);       // low column start address
	twiStop();
	
	lcdRow = row;	//save cursor location
	lcdColumn = column;
}

void lcdSetInvert(BOOL invert)
{
	lcdInvert = invert;
	lcdInvert == TRUE ? lcdSendCommand(OLED_INVERSE) : lcdSendCommand(OLED_NORMAL);
}

BOOL lcdGetInvert() { return lcdInvert; }
	
void lcdFill(uint8_t fillData)
{
	uint8_t m,n;
	for(m = 0; m <= MAX_ROWS; m++) {
		twiStart();
		twiSend(OLED_ADDRESS);
		twiSendCmd(0xb0 + m);              // page0-page1
		twiSendCmd(HIGH_COL_START_ADDR); // high column start address
		twiSendCmd(LOW_COL_START_ADDR);  // low column start address
		twiStop();
		
		twiStart();
		twiSend(OLED_ADDRESS);
		twiSend(DATA_COMMAND);
		for(n = 0; n < LCD_WIDTH; n++) {
			twiSend(fillData);
		}
		twiStop();
	}
}

void lcdClearLine(uint8_t line)
{
	twiSendCmd(LOW_COL_START_ADDR  | 0x0); // low col = 0
	twiSendCmd(HIGH_COL_START_ADDR | 0x0); // hi col = 0
	twiSendCmd(DISPLAY_START_LINE  | 0x0); // line #0

// SSD1306.cpp weird two-wire stuff
//#ifdef TWBR
	//uint8_t twbrbackup = TWBR;
	//TWBR = 18; // upgrade to 400KHz!
//#endif

	// send a bunch of data in one xmission
	twiSendCmd(0xB0 + line);         // set page address
	twiSendCmd(LOW_COL_START_ADDR);  // set lower column address
	twiSendCmd(HIGH_COL_START_ADDR); // set higher column address

	for(uint8_t j = 0; j < 8; j++) {
		twiStart();
		twiSend(OLED_ADDRESS);
		twiSend(DATA_COMMAND);
		
		for (uint8_t k = 0; k < 16; k++) {
			twiSend(0);
		}
		twiStop();
	}
	
//#ifdef TWBR
	//TWBR = twbrbackup;
//#endif
}

void lcdClearScreen()
{
	lcdFill(0);
	lcdSetPos(0, 0);
}

void lcdWriteChar(uint8_t data)
{	//******************************
	twiStart();  //move to lcdPrint
	twiSend(OLED_ADDRESS);  //move to lcdPrint
	twiSend(DATA_COMMAND); //move to lcdPrint
	//******************************
	
	if (data > 0x20 && data <= 0x7f) {
		data -= 0x21; //offsets byte by 0x21, to beginning of ascii codes
		for (uint8_t i = 0; i < 5; i++) {
			twiSend(pgm_read_byte(&font5x8[data][i])); //cycle through and send 5 bytes of char
		}
		twiSend(0); //pad 6th byte because separation between char
	} 
	else if (data == '\n') {
		lcdRow++;
		lcdSetPos(lcdRow, 0);
		return;
	}
	else { //send space for unknown char
		for (uint8_t i = 0; i < 6; i++) {
			twiSend(0);
		}
	}
	lcdColumn += 6;
	
	if (lcdColumn >= MAX_COLS) {
		lcdColumn = 0;
		lcdRow++;
	}
	
	if (lcdRow >= MAX_ROWS) {
		lcdRow = 0;
	}
	
	//******************************
	twiStop();  //move to lcdPrint
	//******************************
}

#define MIN(x, y) x < y ? x : y;

void lcdPrint(const char data[18]) 
{
	uint8_t i = 0;
	uint8_t len = strlen(data);
	
	len = MIN(len, 18); 
	
	while (i < len) { 
		lcdWriteChar(data[i]); 
		i++; 
	}
}

void lcdPrintln(const char data[18]) 
{
	lcdPrint(data);
	lcdWriteChar('\n');
}


//void lcdWriteDigit(byte n)
//{
	//#ifdef TWBR
	//uint8_t twbrbackup = TWBR;
	//TWBR = 18; // upgrade to 400KHz!
	//#endif
	//if (m_font == FONT_SIZE_SMALL) {
		//Wire.beginTransmission(_i2caddr);
		//Wire.write(0x40);
		//if (n <= 9) {
			//n += '0' - 0x21;
			//for (byte i = 0; i < 5; i++) {
				//Wire.write(pgm_read_byte(&font5x8[n][i]));
			//}
			//Wire.write(0);
			//} else {
			//for (byte i = 0; i < 6; i++) {
				//Wire.write(0);
			//}
		//}
		//Wire.endTransmission();
		//m_col += 6;
		//} else if (m_font == FONT_SIZE_MEDIUM) {
		//write(n <= 9 ? ('0' + n) : ' ');
		//#ifndef MEMORY_SAVING
		//} else if (m_font == FONT_SIZE_LARGE) {
		//if (n <= 9) {
			//byte i;
			//twiSendCmd(0xB0 + m_row);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (i = 0; i < 16; i ++) {
				//byte d = pgm_read_byte(&digits16x16[n][i]);
				//Wire.write(d);
				//if (m_flags & FLAG_PIXEL_DOUBLE_H) Wire.write(d);
			//}
			//Wire.endTransmission();
//
			//twiSendCmd(0xB0 + m_row + 1);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (; i < 32; i ++) {
				//byte d = pgm_read_byte(&digits16x16[n][i]);
				//Wire.write(d);
				//if (m_flags & FLAG_PIXEL_DOUBLE_H) Wire.write(d);
			//}
			//Wire.endTransmission();
			//} else {
			//twiSendCmd(0xB0 + m_row);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--) {
				//Wire.write(0);
			//}
			//Wire.endTransmission();
//
			//twiSendCmd(0xB0 + m_row + 1);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--) {
				//Wire.write(0);
			//}
			//Wire.endTransmission();
		//}
		//m_col += (m_flags & FLAG_PIXEL_DOUBLE_H) ? 30 : 16;
		//#endif
		//} else {
		//if (n <= 9) {
			//byte i;
			//twiSendCmd(0xB0 + m_row);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (i = 0; i < 16; i ++) {
				//byte d = pgm_read_byte(&digits16x24[n][i * 3]);
				//Wire.write(d);
				//if (m_flags & FLAG_PIXEL_DOUBLE_H) Wire.write(d);
			//}
			//Wire.endTransmission();
//
			//twiSendCmd(0xB0 + m_row + 1);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (i = 0; i < 16; i ++) {
				//byte d = pgm_read_byte(&digits16x24[n][i * 3 + 1]);
				//Wire.write(d);
				//if (m_flags & FLAG_PIXEL_DOUBLE_H) Wire.write(d);
			//}
			//Wire.endTransmission();
//
			//twiSendCmd(0xB0 + m_row + 2);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (i = 0; i < 16; i ++) {
				//byte d = pgm_read_byte(&digits16x24[n][i * 3 + 2]);
				//Wire.write(d);
				//if (m_flags & FLAG_PIXEL_DOUBLE_H) Wire.write(d);
			//}
			//Wire.endTransmission();
			//} else {
			//twiSendCmd(0xB0 + m_row);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--) {
				//Wire.write(0);
			//}
			//Wire.endTransmission();
//
			//twiSendCmd(0xB0 + m_row + 1);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--) {
				//Wire.write(0);
			//}
			//Wire.endTransmission();
//
			//twiSendCmd(0xB0 + m_row + 2);//set page address
			//twiSendCmd(m_col & 0xf);//set lower column address
			//twiSendCmd(0x10 | (m_col >> 4));//set higher column address
//
			//Wire.beginTransmission(_i2caddr);
			//Wire.write(0x40);
			//for (byte i = (m_flags & FLAG_PIXEL_DOUBLE_H) ? 32 : 16; i > 0; i--) {
				//Wire.write(0);
			//}
			//Wire.endTransmission();
		//}
		//m_col += (m_flags & FLAG_PIXEL_DOUBLE_H) ? 30 : 16;
	//}
	//#ifdef TWBR
	//TWBR = twbrbackup;
	//#endif
//}
//
//void lcdWriteInt(uint16_t value, int8_t padding)
//{
	//uint16_t den = 10000;
	//for (int8_t i = 5; i > 0; i--) {
		//byte v = (byte)(value / den);
		//value -= v * den;
		//den /= 10;
		//if (v == 0 && padding && den) {
			//if (padding >= i) {
				//writeDigit((m_flags & FLAG_PAD_ZERO) ? 0 : -1);
			//}
			//continue;
		//}
		//padding = 0;
		//writeDigit(v);
	//}
//}
//
//void lcdWriteLong(uint32_t value, int8_t padding)
//{
	//uint32_t den = 1000000000;
	//for (int8_t i = 10; i > 0; i--) {
		//byte v = (byte)(value / den);
		//value -= v * den;
		//den /= 10;
		//if (v == 0 && padding && den) {
			//if (padding >= i) {
				//writeDigit((m_flags & FLAG_PAD_ZERO) ? 0 : -1);
			//}
			//continue;
		//}
		//padding = 0;
		//writeDigit(v);
	//}
//}

void lcdDraw(const PROGMEM uint8_t* buffer, uint8_t width, uint8_t height)
{
	twiSendCmd(LOW_COL_START_ADDR  | 0x0); // low col = 0
	twiSendCmd(HIGH_COL_START_ADDR | 0x0); // hi col = 0
	twiSendCmd(DISPLAY_START_LINE  | 0x0); // line #0

	const PROGMEM uint8_t *p = buffer;
	height >>= 3;
	width >>= 3;

// SSD1306.cpp weird two-wire stuff
//#ifdef TWBR
	//uint8_t twbrbackup = TWBR;
	//TWBR = 18; // upgrade to 400KHz!
//#endif

	for (uint8_t i = 0; i < height; i++) {
		// send a bunch of data in one xmission
		twiSendCmd(0xB0 + i + lcdRow);      // set page address
		twiSendCmd(lcdColumn & 0xf);        // set lower column address
		twiSendCmd(0x10 | (lcdColumn >> 4));// set higher column address

		for(uint8_t j = 0; j < 8; j++) {
			twiStart();
			twiSend(OLED_ADDRESS);
			twiSend(DATA_COMMAND);
			
			for(uint8_t n = 0; n < LCD_WIDTH; n++) {
				twiSend(pgm_read_byte(p));
			}
			twiStop();
		}
	}
	
	lcdColumn += width;

//#ifdef TWBR
	//TWBR = twbrbackup;
//#endif
}

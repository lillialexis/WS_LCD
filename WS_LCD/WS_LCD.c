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
	TWBR = 0x32; //set SCL to 400kHz
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
		twiSendCmd(CLOCK_DIVIDE_RATIO);   // set ratio 128x64:80 96x16:f0
	twiSendCmd(0xa8);       // set multiplex ratio (screen lines 1 to 64)
		twiSendCmd(MULTIPLEX_RATIO); // set 16:0x0f 64:0x3f
	twiSendCmd(0xd3);       // set display offset
		twiSendCmd(0);      // not offset
	twiSendCmd(DISPLAY_START_LINE); // set display start line
	//twiSendCmd(0xd8);       // set display mode
		//twiSendCmd(0x05);   // low power
	twiSendCmd(OLED_NORMAL);
	twiSendCmd(OLED_RUN);
	twiSendCmd(SEGMENT_REMAP); // --set segment re-map 96 to 1
	twiSendCmd(0xC8);       // --Set COM Output Scan Direction 16 to 1
	twiSendCmd(0xda);       // --set com pins hardware configuration
		twiSendCmd(COM_PIN_RATIO); // set ratio 128x64:0x12 96x16:0x02
	twiSendCmd(0x81);       // --set contrast control register
		twiSendCmd(CONTRAST_LEVEL);
	twiSendCmd(0xd9);       // --set pre-charge period
		twiSendCmd(PRE_CHARGE_PERIOD);   // set ratio 128x64:0xf1 96x16:0x22
	twiSendCmd(0xdb);       // --set vcomh
		twiSendCmd(VCOM_DETECT); // --0.77vref
	twiSendCmd(0x20);       // Set Memory Addressing Mode
		twiSendCmd(0x00);   // 00, Horizontal Addressing Mode; 01, Vertical Addressing Mode; 10, Page Addressing Mode (RESET); 11, Invalid
	twiSendCmd(0x8d);       // charge pump control
		twiSendCmd(0x14);   // 0x14:Run 0x10:off
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

#define MIN(x, y) x < y ? x : y;
#define MAX(x, y) x > y ? x : y;

void internal_lcdSetPos(uint8_t row, uint8_t column)
{
	
	
}

void lcdSetPos(uint8_t row, uint8_t column)
{
	row = MIN(row, MAX_ROWS);
	column = MIN(column, LCD_WIDTH);
	
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
	
void lcdFill(char fillData)
{
	uint8_t m,n;
	for(m = 0; m <= MAX_ROWS; m++) {
		//twiStart();
		//twiSend(OLED_ADDRESS);
		//twiSendCmd(0xb0 + m);            // page0-page1
		//twiSendCmd(HIGH_COL_START_ADDR); // high column start address
		//twiSendCmd(LOW_COL_START_ADDR);  // low column start address
		//twiStop();
		
		lcdSetPos(m, 0);
		
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
	if (line >= MAX_ROWS) return;
	
	//twiStart();
	//twiSend(OLED_ADDRESS);
	//twiSendCmd(0xb0 + line);         // page0-page1
	//twiSendCmd(HIGH_COL_START_ADDR); // high column start address
	//twiSendCmd(LOW_COL_START_ADDR);  // low column start address
	//twiStop();
	
	lcdSetPos(line, 0);
			
	twiStart();
	twiSend(OLED_ADDRESS);
	twiSend(DATA_COMMAND);
	for(int n = 0; n < LCD_WIDTH; n++) {
		twiSend(0);
	}
	twiStop();
}

void lcdClearScreen()
{
	lcdFill(0);
	lcdSetPos(0, 0);
}

void lcdWriteChar(char data)
{	
	twiStart(); // maybe move these later to lcdprint, etc.
	twiSend(OLED_ADDRESS); // maybe move these later to lcdprint, etc.
	twiSend(DATA_COMMAND); // maybe move these later to lcdprint, etc.
	
	if (data >= ASCII_OFFSET_LOW && data <= ASCII_OFFSET_HIGH) {
		data -= ASCII_OFFSET_LOW; // offsets byte by 0x20, to beginning of ascii codes
		for (uint8_t i = 0; i < SMALL_FONT_CHARACTER_W; i++) {
			twiSend(pgm_read_byte(&font5x8[data][i])); // cycle through and send 5 bytes of char
		}
		twiSend(0); // pad 6th byte because separation between char
		
		lcdColumn += SMALL_FONT_CHARACTER_W + H_PADDING;
	} 
	else if (data == '\n') {
		lcdRow++;
		lcdColumn = 0;
	}
	else { // send space for unknown char
		for (uint8_t i = 0; i < SMALL_FONT_CHARACTER_W; i++) {
			twiSend(pgm_read_byte(&unknown5x8[i])); // cycle through and send 5 bytes of the unknown char
		}
		twiSend(0); // pad 6th byte because separation between char
	
		lcdColumn += SMALL_FONT_CHARACTER_W + H_PADDING;
	}
	
	if (lcdColumn >= LCD_WIDTH) {
		lcdColumn = 0;
		lcdRow++;
	}
	
	if (lcdRow >= MAX_ROWS) {
		lcdRow = 0;
	}
	
	lcdSetPos(lcdRow, lcdColumn);

	twiStop(); // maybe move these later to lcdprint, etc.
}

void lcdPrint(char *buffer)
{
	//twiStart();
	//twiSend(OLED_ADDRESS);
	//twiSend(DATA_COMMAND);
	
	uint8_t i = 0;
	uint8_t len = strlen(buffer);
	
	while (i < len) { 
		lcdWriteChar(buffer[i]); 
		i++; 
	}
	
	//twiStop();
}

void lcdPrintln(char *buffer)
{
	lcdPrint(buffer);
	lcdWriteChar('\n');
}


void lcdReplaceLine(uint8_t line, char *buffer)
{
	lcdClearLine(line);
	lcdSetPos(line, 0);
	lcdPrintln(buffer);
}

void lcdPrint_P(const PROGMEM char *buffer)
{
	//twiStart();
	//twiSend(OLED_ADDRESS);
	//twiSend(DATA_COMMAND);
	
	while (pgm_read_byte(buffer) != 0x00)
		lcdWriteChar(pgm_read_byte(buffer++));
		
	//twiStop();
}

void lcdPrintln_P(const PROGMEM char *buffer)
{
	lcdPrint_P(buffer);
	lcdWriteChar('\n');
}

void lcdReplaceLine_P(uint8_t line, const PROGMEM char *buffer)
{
	lcdClearLine(line);
	lcdSetPos(line, 0);
	lcdPrintln_P(buffer);
}

void lcdDraw(const PROGMEM uint8_t* buffer, uint8_t width, uint8_t height)
{
	twiSendCmd(LOW_COL_START_ADDR  | 0x0); // low col = 0
	twiSendCmd(HIGH_COL_START_ADDR | 0x0); // hi col = 0
	twiSendCmd(DISPLAY_START_LINE  | 0x0); // line #0

	const PROGMEM uint8_t *p = buffer;
	height >>= 3;
	width >>= 3;

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
}

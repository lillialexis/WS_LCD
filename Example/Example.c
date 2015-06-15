/*
 * Example.c
 *
 * Created: 1/22/2015 5:10:41 PM
 *  Author: dev-box
 */ 

// Header files
#include <avr/io.h>
#include <stdio.h>

#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/interrupt.h>

//#define LCD_RESOLUTION 12864
#define LCD_RESOLUTION 9616

#include "../WS_LCD/WS_LCD.h"


// TO USE:
// 1. Add a test method; you can call it whatever.
// 2. Add a pointer to your method at the end of the tests[] array in the main() method {..., &<name_of_method>};
// 3. Update the starting index TEST_START_INDEX to the index of the first test you want to run (default is 0)
// 4. Update the number of tests to run NUMBER_TO_RUN to run that many tests (default is max number of tests)
// 5. Run the example

// MAKE SURE YOU UPDATE THESE SO THAT THE LOOP CALLS YOUR NEWLY ADDED TEST
#define TEST_START_INDEX 11
#define NUMBER_TO_RUN    1
#define TEST_DELAY       3000


// YOU CAN ADD MORE TESTS HERE
void exampleTest()
{
	
}

void test_lcdFill()
{
	uint8_t fills[] = { 0x01,   // ? ? ? ?  ? ? ? ?  
   					    0x04,   // ? ? ? ?  ? ? ? ?
   					    0x10,   // ? ? ? ?  ? ? ? ?
   					    0x20,   // ? ? ? ?  ? ? ? ?
   					    0xAA,   // ? ? ? ?  ? ? ? ?
   					    0x55,   // ? ? ? ?  ? ? ? ? 
   					    0xff,   // ? ? ? ?  ? ? ? ? 
   					    0x00 }; // ? ? ? ?  ? ? ? ? 
	
	for (uint8_t i = 0; i < 8; i++)
	{
		lcdFill(fills[i]);
		
		_delay_ms(500);	
	}	
}

void test_lcdPrintln_noWrap() // Test print line only to max rows
{
	lcdSetPos(0, 0);

	lcdPrintln("1");
	lcdPrintln("2");
	lcdPrintln("3");
	lcdPrintln("4");
	lcdPrintln("5");
	lcdPrintln("6");
	lcdPrintln("7");
	lcdPrintln("8");
	
	_delay_ms(5000);
}

void test_lcdPrintln_wrap()
{
	lcdSetInvert(TRUE);
		
	lcdSetPos(0, 0);

	lcdPrintln("1");
	lcdPrintln("2");
	lcdPrintln("3");
	lcdPrintln("4");
	lcdPrintln("5");
	lcdPrintln("6");
	lcdPrintln("7");
	lcdPrintln("8");
	lcdPrintln("9");
	lcdPrintln("10");
	lcdPrintln("11");
		
	_delay_ms(5000);
}

void test_lcdClearLine()
{
	lcdFill(0xff);
	
	lcdClearLine(0);
	_delay_ms(1000);
	
	lcdClearLine(2);
	_delay_ms(1000);

	lcdClearLine(4);
	_delay_ms(1000);
	
	lcdClearLine(6);
	_delay_ms(1000);
	
	lcdClearLine(8);
	_delay_ms(1000);
	
	lcdClearLine(100);
	_delay_ms(1000);
	
	lcdClearLine(7);		
	_delay_ms(300);

	lcdClearLine(5);
	_delay_ms(300);

	lcdClearLine(3);
	_delay_ms(300);

	lcdClearLine(1);
	_delay_ms(300);
}

void test_lcdPrint_wrapLine()
{
	lcdPrint("012345678901234567890123456789");
	_delay_ms(5000);	
}

void test_lcdPrint_wrapScreen()
{
	lcdPrint("1aaaaaaaaaaaaaaaaaaaaA2bbbbbbbbbbbbbbbbbbbbB3ccccccccccccccccccccC4ddddddddddddddddddddD5eeeeeeeeeeeeeeeeeeeeE6ffffffffffffffffffffF7ggggggggggggggggggggG8hhhhhhhhhhhhhhhhhhhhH9xxxxxxxxxxxxxx");
	_delay_ms(5000);
}

char const FlashString[] PROGMEM = "This is a string held completely in flash memory.";
void test_lcdPrint_P()
{
	lcdPrint_P(PSTR("This is a PROGMEM String. "));
	lcdPrint_P(PSTR("This is too. "));
	lcdPrint_P(FlashString);
	
	_delay_ms(3000);
}

void test_lcdPrintln_P()
{
	lcdPrintln_P(FlashString);
	lcdPrintln_P(PSTR("This is too. "));
	lcdPrintln_P(PSTR("This is a PROGMEM String. "));
	
	_delay_ms(3000);
}

void test_sprintf()
{
	int i = 100;
	
	char dec[50];
	char hex[50];
	
	sprintf(dec, "dec: %d", i);
	lcdPrintln(dec);
	
	sprintf(hex, "hex: 0x%x", i);
	lcdPrintln(hex);

	_delay_ms(3000);	
}

char const hexFS[] PROGMEM = "hex: 0x%x";
void test_sprintf_P()
{
	int i = 200;
	
	char dec[50];
	char hex[50];
	
	sprintf_P(dec, PSTR("dec: %d"), i);
	lcdPrintln(dec);
	
	sprintf_P(hex, hexFS, i);
	lcdPrintln(hex);

	_delay_ms(3000);
}

void test_sizes()
{
	if (sizeof(char) > sizeof(uint8_t))
		lcdPrint("char is bigger");
	else if (sizeof(char) == sizeof(uint8_t))
		lcdPrint("char is same");
	else
		lcdPrint("char is smaller");
		
	_delay_ms(2000);
}
void showTitle (char * useMenu(uint8_t), uint8_t pos)
{
	lcdClearLine(0);
	lcdSetPos(0, 0);
	lcdPrint_P(useMenu(pos));
}

char menuContent(uint8_t pos)
{
	if (pos == 1)              {return(PSTR("for some"));} //returns menu title
	else if (pos == 2)              {return(PSTR("silly reason"));} //returns menu title
	else if (pos == 3)              {return(PSTR("this has no "));} //returns menu title
	else if (pos == 4)              {return(PSTR("problems"));} //returns menu title
	else {return("");}
}

void test_jesses_problem (){
	showTitle(menuContent, 1);
	_delay_ms(1500);
	showTitle(menuContent, 2);
	_delay_ms(1500);
	showTitle(menuContent, 3);
	_delay_ms(1500);
	showTitle(menuContent, 4);
	_delay_ms(3000);
}


	
// ADD THE NAME OF YOUR TEST TO THE ARRAY (PROCEEDED BY AMPERSAND)
void (*tests[])(void) = { &test_lcdFill, 
						  &test_lcdPrintln_noWrap, 
						  &test_lcdPrintln_wrap, 
						  &test_lcdClearLine,
						  &test_lcdPrint_wrapLine, 
						  &test_lcdPrint_wrapScreen, 
						  &test_lcdPrint_P,
						  &test_lcdPrintln_P,
						  &test_sprintf,
						  &test_sprintf_P,
						  &test_sizes, 
						  &test_jesses_problem};

void setUp()
{
	lcdClearScreen();
	
	int c = (lcdGetMaxCols() - strlen("Starting Test...")) / 2;
	int r = lcdGetMaxRows() / 2;
	
	lcdSetPos(r, c);
	lcdPrintln("Starting Test...");
	
	_delay_ms(1000);
	
	lcdClearScreen();
	lcdSetPos(0, 0);
}

void tearDown()
{
	if (lcdGetInvert()) lcdSetInvert(FALSE);
	
	lcdClearScreen();
	
	int c = (lcdGetMaxCols() - strlen("Test Completed...")) / 2;
	int r = lcdGetMaxRows() / 2;
	
	lcdSetPos(r, c);
	lcdPrintln("Test Complete...");
	
	_delay_ms(TEST_DELAY);
}

void beginTesting()
{
	lcdInit();

	if (lcdGetInvert()) lcdSetInvert(FALSE);
	
	lcdFill(0xAA);
	lcdSetPos(0, 0);
	lcdPrintln("Hello world!");
	lcdPrintln("Starting tests...");

	_delay_ms(500);
	
	uint8_t n[] = "3... 2... 1... ";
	
	for (uint8_t i = 0; i < strlen(n); i++)
	{
		lcdWriteChar(n[i]);
		
		if (i % 5 == 4)
			_delay_ms(600);
		else
			_delay_ms(100);
	}
	
	_delay_ms(500);
}

void endTesting()
{
	lcdFill(0);
	
	if (lcdGetInvert()) lcdSetInvert(FALSE);
	
	for (int i = 0; i < 6; i++)
	{
		lcdSetInvert(!lcdGetInvert());
		_delay_ms(150);
	}
	
	int c = (lcdGetMaxCols() - strlen("Tests Completed!")) / 2;
	int r = lcdGetMaxRows() / 2;
	
	lcdSetPos(r, c);
	lcdPrintln("Tests Completed!");
}

int main(void)
{
	//// Time to activate the debug terminal
	_delay_ms(5000);

	beginTesting();
	
	// YOU CAN CHANGE THE TEST_START_INDEX AND NUMBER_OF_TESTS ABOVE, IF YOU DON'T WANT TO RUN THROUGH ALL OF THEM
	for (int i = TEST_START_INDEX; i < TEST_START_INDEX + NUMBER_TO_RUN; i++)
	{
		// SET-UP METHOD IS CALLED BEFORE EACH TEST; IF YOU WANT TO CHANGE THE SCREEN-FILL FOR THE TEST, YOU CAN DO SO ABOVE
		setUp();
		
		void (*currentTest)(void) = tests[i];
		currentTest();
				
		// TEAR-DOWN METHOD IS AUTOMATICALLY CALLED AFTER EACH TEST
		tearDown();
	}
	
	endTesting();
	
	while (1)
	{
			
	}
}
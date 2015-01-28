/*
 * Example.c
 *
 * Created: 1/22/2015 5:10:41 PM
 *  Author: dev-box
 */ 

// Header files
#include <avr/io.h>

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
// 5. If you want to add a fill for your method, you can do so in the fillForTest() method. (Note: the fill is based 
//        off of the index of the test method in the array; changing which tests are run will change which fills are used)
// 6. Run the example

// MAKE SURE YOU UPDATE THESE SO THAT THE LOOP CALLS YOUR NEWLY ADDED TEST
#define TEST_START_INDEX 0
#define NUMBER_TO_RUN    4

// YOU CAN ADD MORE TESTS HERE
void exampleTest()
{
	
}

void test1()
{
	lcdSetInvert(TRUE);
	
	lcdSetPos(0, 0);
	lcdPrintln("Hello world,");
	lcdPrintln("Muthafuckas!!!");
	lcdPrintln("Hello world,");
	lcdPrintln("1fdafasfasf");
	lcdPrintln("2");
	lcdPrintln("3");
	lcdPrintln("4fdafasfasfasf");
	lcdPrintln("5");
	lcdPrintln("6fdafsa");
	lcdPrintln("7fda");
	lcdPrintln("8fdsa");
	lcdPrintln("9");
	lcdPrintln("10");
	lcdPrintln("11");
	
	//lcdClearLine(0);
	//lcdClearLine(5);
	//lcdSetPos(5, 5);
	
	//uint8_t r = lcdGetCurrentRow() + 15;
	//uint8_t c = lcdGetCurrentCol() + 15;
	//
	//lcdWriteChar(r);
	//lcdWriteChar(c);
}

void test2()
{
	
}

void test3()
{
	
}

void test4()
{
	
}

void setUp(uint8_t fill)
{
	lcdFill(fill);
}

void tearDown()
{
	lcdClearScreen();
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
	
	uint8_t n[] = {'3', '2', '1'};
	
	for (uint8_t i = 0; i < strlen(n); i++)
	{
		lcdWriteChar(n[i]);
		_delay_ms(1000);
	}
	
	_delay_ms(500);
}

void endTesting()
{
	lcdFill(0);
	
	if (lcdGetInvert()) lcdSetInvert(FALSE);
	
	for (int i = 0; i < 10; i++)
	{
		lcdSetInvert(!lcdGetInvert());
		_delay_ms(2000);
	}
	
	int c = (lcdGetMaxCols() - strlen("Tests Completed!")) / 2;
	int r = lcdGetMaxRows() / 2;
	
	lcdSetPos(r, c);
	lcdPrintln("Tests Completed!");
}

uint8_t fillForTest(int testNum)
{
	switch (testNum)
	{
		case 0: return '1';
		case 1: return '2';
		case 2: return '3';
		case 3: return '4';
		default: return '+';
	}
}

int main(void)
{
	//// Time to activate the debug terminal
	_delay_ms(5000);

	beginTesting();
	
	// ADD THE NAME OF YOUR TEST TO THE ARRAY (PROCEEDED BY AMPERSAND)
	void (*tests[])(void) = { &test1, &test2, &test3, &test4 };
	
	// YOU CAN CHANGE THE TEST_START_INDEX AND NUMBER_OF_TESTS ABOVE, IF YOU DON'T WANT TO RUN THROUGH ALL OF THEM
	for (int i = TEST_START_INDEX; i < NUMBER_TO_RUN; i++)
	{
		// SET-UP METHOD IS CALLED BEFORE EACH TEST; IF YOU WANT TO CHANGE THE SCREEN-FILL FOR THE TEST, YOU CAN DO SO ABOVE
		setUp(fillForTest(i));
		
		void (*currentTest)(void) = tests[i];
		currentTest();
		
		_delay_ms(1000);
		
		// TEAR-DOWN METHOD IS AUTOMATICALLY CALLED AFTER EACH TEST
		tearDown();
	}
	
	endTesting();
	
	while (1)
	{
			
	}
}
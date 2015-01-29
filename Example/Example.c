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
#define TEST_START_INDEX 2
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

//void test_lcdFillChar()
//{
	//lcdFillChar(0x00);	
	//_delay_ms(3000);
	//
	//lcdFillChar("X");
	//_delay_ms(3000);
	//
	////uint8_t fills[] = { 0x00,
						////' ',
						////'+',
						////0x23,
						////"x" };
	////
	////for (uint8_t i = 0; i < 5; i++)
	////{
		////lcdFillChar(fills[i]);
		////
		////_delay_ms(3000);
	////}
//}

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
	
	//lcdClearLine(0);
	//lcdClearLine(5);
	//lcdSetPos(5, 5);
		
	//uint8_t r = lcdGetCurrentRow() + 15;
	//uint8_t c = lcdGetCurrentCol() + 15;
	//
	//lcdWriteChar(r);
	//lcdWriteChar(c);
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

void test4()
{
	
}

// ADD THE NAME OF YOUR TEST TO THE ARRAY (PROCEEDED BY AMPERSAND)
void (*tests[])(void) = { &test_lcdFill, &test_lcdPrintln_noWrap, &test_lcdPrintln_wrap, &test_lcdClearLine, &test4 };

void setUp()//uint8_t fill)
{
	lcdClearScreen();
	
	int c = (lcdGetMaxCols() - strlen("Starting Test...")) / 2;
	int r = lcdGetMaxRows() / 2;
	
	lcdSetPos(r, c);
	lcdPrintln("Starting Test...");
	
	_delay_ms(1500);
	
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
	
	//_delay_ms(1500);
	//
	//lcdClearScreen();
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

//uint8_t fillForTest(int testNum)
//{
	//switch (testNum)
	//{
		//case 0: return '1';
		//case 1: return '2';
		//case 2: return '3';
		//case 3: return '4';
		//default: return '+';
	//}
//}

int main(void)
{
	//// Time to activate the debug terminal
	_delay_ms(5000);

	beginTesting();
	
	//// ADD THE NAME OF YOUR TEST TO THE ARRAY (PROCEEDED BY AMPERSAND)
	//void (*tests[])(void) = { &test1, &test2, &test3, &test4 };
	
	// YOU CAN CHANGE THE TEST_START_INDEX AND NUMBER_OF_TESTS ABOVE, IF YOU DON'T WANT TO RUN THROUGH ALL OF THEM
	for (int i = TEST_START_INDEX; i < TEST_START_INDEX + NUMBER_TO_RUN; i++)
	{
		// SET-UP METHOD IS CALLED BEFORE EACH TEST; IF YOU WANT TO CHANGE THE SCREEN-FILL FOR THE TEST, YOU CAN DO SO ABOVE
		setUp();//fillForTest(i));
		
		void (*currentTest)(void) = tests[i];
		currentTest();
				
		// TEAR-DOWN METHOD IS AUTOMATICALLY CALLED AFTER EACH TEST
		tearDown();
	
		_delay_ms(TEST_DELAY);
	}
	
	endTesting();
	
	while (1)
	{
			
	}
}
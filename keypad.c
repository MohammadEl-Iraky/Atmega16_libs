/*****************************************************************************************************
*  
*  keypad.c
*  Created on: Feb 24, 2020
*  Author: Mohammad Metwally
*
****************************************************************************************************/

#include "keypad.h"

/*****************************************************************************************************
*  										Private	Function Prototypes
*****************************************************************************************************/

#if COLS_NO == 3
	static uint8 KEYPAD_adjust3x3( uint8 button );
#elif COLS_NO == 4
	static uint8 KEYPAD_adjust4x4( uint8 button );
#endif

/*****************************************************************************************************
*  											Function definitions
*****************************************************************************************************/

void KEYPAD_init( void )
{
	/* Enable Pull-up resistors for row pins */
	KEYPAD_PORT = (0x0F<<ROWS_OFFSET);

	/* Set all Pins as input */
	KEYPAD_DDR = 0x00;
}

uint8 KEYPAD_getPressedKey( void )
{
	uint8 col = 0;	/* Used to loop for the columns */
	uint8 row = 0;	/* Used to loop for the rows */

	while(1)
	{
		/* Loop for the columns and turn each one LOW at a time while keeping the others at High-Z state */
		for( col=0; col<COLS_NO; col++ )
		{
			/* Turn one of the column pins as output while keeping the other at High-Z state */
			KEYPAD_DDR = ( (1<<COLS_OFFSET)<<col );

			/* Loop for each row and find which one is LOW */
			for( row=0; row<ROWS_NO; row++ )
			{
				if( !(KEYPAD_PIN & ( (1<<ROWS_OFFSET)<<row)) )
				{
					#if COLS_NO == 3
						return KEYPAD_adjust4x3( (row*COLS_NO)+col+1 );
					#elif COLS_NO == 4
						return KEYPAD_adjust4x4( (row*COLS_NO)+col+1 );
					#endif
				}
				else
				{

				}
			}
		}
	}
}

#if COLS_NO == 3

static uint8 KEYPAD_adjust3x3( uint8 button )
{
	uint8 pressed_key = button;

	switch(button)
	{
		case 10: pressed_key = '*';
				 break;
		case 11: pressed_key = 0;
				 break;
		case 12: pressed_key = '#';
				 break;
		default: pressed_key = button_number;
				 break;
	}

	return pressed_key;
}


#elif COLS_NO == 4

static uint8 KEYPAD_adjust4x4( uint8 button )
{
	uint8 pressed_key = button;

	switch(button)
	{
		case 1: pressed_key = 7;
				break;
		case 2: pressed_key = 8;
				break;
		case 3: pressed_key = 9;
				break;
		case 4: pressed_key = '%';
				break;
		case 5: pressed_key = 4;
				break;
		case 6: pressed_key = 5;
				break;
		case 7: pressed_key = 6;
				break;
		case 8: pressed_key = '*';
				break;
		case 9: pressed_key = 1;
				break;
		case 10: pressed_key = 2;
				break;
		case 11: pressed_key = 3;
				break;
		case 12: pressed_key = '-';
				break;
		case 13: pressed_key = 13;  /* ASCII of Enter */
				break;
		case 14: pressed_key = 0;
				break;
		case 15: pressed_key = '=';
				break;
		case 16: pressed_key = '+';
				break;
		default: break;
	}
	return pressed_key;
}

#endif

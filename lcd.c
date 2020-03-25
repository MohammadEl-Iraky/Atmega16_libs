/*****************************************************************************************************
*  
*  lcd.c
*  Created on: Feb 24, 2020
*  Author: Mohammad Metwally
*
****************************************************************************************************/

#include "lcd.h"

/*****************************************************************************************************
*  											Function definitions
*****************************************************************************************************/

void LCD_init( void )
{
	/* Set LCD data pins as Output */
#if (DATA_BITS_MODE == 8)
	LCD_DATA_DDR = 0xFF;
#elif DATA_BITS_MODE == 4
	#ifdef	UPPER_PORT_PINS
	LCD_DATA_DDR |= 0xF0;
	#else
	LCD_DATA_DDR |= 0x0F;
	#endif
#endif

	/* Set LCD CTRL pins as output */
	LCD_CTRL_DDR |= (1<<RS)|(1<<RW)|(1<<EN);
	/* Select data 8 or 4 bit mode */
#if (DATA_BITS_MODE == 8 )
	LCD_sendCommand( EIGHT_BITS_TWO_LINE );
#elif (DATA_BITS_MODE == 4 )
	LCD_sendCommand( FOUR_BITS_MODE );
	LCD_sendCommand( FOUR_BITS_TWO_LINE );
#endif
	/* Clear the LCD screen */
	LCD_sendCommand( CLEAR_SCREEN );
	/* Turn on LCD display with Blinking cursor */
	LCD_sendCommand( TURN_ON_DISPLAY );
}

void LCD_sendCommand( uint8 command )
{
	/* Set RW pin to LOW for write operation &  Set RS to low to select the command register */
	LCD_CTRL_PORT &= (~((1<<RW)|(1<<RS)));
	/* tAS = 40ns ( Setup time before Enable pin going high ) */
	_delay_ms(1);
	LCD_CTRL_PORT |= (1<<EN);		/* Set Enable pin high */
	/* tPWH - tDSW = 230ns - 80ns = 150ns ( Wait after Enable going high before sending the command ) */
	_delay_ms(1);
#if ( DATA_BITS_MODE == 8 )
	LCD_DATA_PORT = command;
#elif (DATA_BITS_MODE == 4 )
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = ( LCD_DATA_PORT & 0x0F ) | ( command & 0xF0 );	/* Upper 4 bits first */
#else
	LCD_DATA_PORT = ( LCD_DATA_PORT & 0xF0 ) | ( command>>4 ); /* Upper 4 bits first */
#endif
#endif
	/* tDSW = 80ns ( Data setup time ) */
	_delay_ms(1);
	LCD_CTRL_PORT &= (~(1<<EN));	/* Set Enable pin LOW */
#if ( DATA_BITS_MODE == 4 )
	/* tAH = 10ns ( Hold time for RS and R/W after Enable became LOW */
	_delay_ms(1);
	LCD_CTRL_PORT |= (1<<EN);
	/* tPWH - tDSW = 230ns - 80ns = 150ns ( Wait after Enable going high before sending the command ) */
	_delay_ms(1);
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = ( LCD_DATA_PORT & 0x0F ) | ( (command<<4) );	/* Lower 4 bits */
#else
	LCD_DATA_PORT = ( LCD_DATA_PORT & 0xF0 ) | ( (command & 0x0F) ); /* Lower 4 bits */
#endif
	/* tDSW = 80ns ( Data setup time ) */
	_delay_ms(1);
	LCD_CTRL_PORT &= (~(1<<EN));	/* Set Enable pin LOW */
#endif
	/* tAH = 10ns ( Hold time for RS and R/W after Enable became LOW */
	_delay_ms(1);
	LCD_CTRL_PORT |= (1<<RW)|(1<<RS);	/* Set back RS and RW to high */
}


void LCD_displayCharacter( uint8 data )
{
	/* Set RW pin to LOW for write operation */
	LCD_CTRL_PORT &= (~(1<<RW));
	/* Set RS high to select the data register */
	LCD_CTRL_PORT |= (1<<RS);
	_delay_ms(1);
	LCD_CTRL_PORT |= (1<<EN);		/* Set Enable pin high */
	/* tPWH - tDSW = 230ns - 80ns = 150ns ( Wait after Enable going high before sending the command ) */
	_delay_ms(1);
#if ( DATA_BITS_MODE == 8 )
	LCD_DATA_PORT = data;
#elif (DATA_BITS_MODE == 4 )
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = ( LCD_DATA_PORT & 0x0F ) | ( data & 0xF0 );	/* Upper 4 bits first */
#else
	LCD_DATA_PORT = ( LCD_DATA_PORT & 0xF0 ) | ( data>>4 ); /* Upper 4 bits first */
#endif
#endif
	/* tDSW = 80ns ( Data setup time ) */
	_delay_ms(1);
	LCD_CTRL_PORT &= (~(1<<EN));	/* Set Enable pin LOW */
#if ( DATA_BITS_MODE == 4 )
	/* tAH = 10ns ( Hold time for RS and R/W after Enable became LOW */
	_delay_ms(1);
	LCD_CTRL_PORT |= (1<<EN);
	/* tPWH - tDSW = 230ns - 80ns = 150ns ( Wait after Enable going high before sending the command ) */
	_delay_ms(1);
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = ( LCD_DATA_PORT & 0x0F ) | ( (data<<4) );	/* Lower 4 bits */
#else
	LCD_DATA_PORT = ( LCD_DATA_PORT & 0xF0 ) | ( (data & 0x0F) ); /* Lower 4 bits */
#endif
	/* tDSW = 80ns ( Data setup time ) */
	_delay_ms(1);
	LCD_CTRL_PORT &= (~(1<<EN));	/* Set Enable pin LOW */
#endif
	/* tAH = 10ns ( Hold time for RS and R/W after Enable became LOW */
	_delay_ms(1);
	LCD_CTRL_PORT |= (1<<RW)|(1<<RS);	/* Set back RS and RW to high */
}

void LCD_displayString( const char * str )
{
	uint8 i = 0;
	/* Loop until the end of the string */
	while( str[i] != '\0' )
	{
		LCD_displayCharacter( str[i] );
		i++;
	}
}

void LCD_goToRowCol( uint8 row, uint8 col)
{
	uint8 address=0;
	switch( row )
	{
		case 0: address = col;
				break;
		case 1: address = 0x40 + col;
				break;
		case 3: address = 0x10 + col;
				break;
		case 4: address = 0x50 + col;
	}
	address |= CURSOR_CHANGE;	/* Set bit 7 to High to change the cursor */
	LCD_sendCommand( address );
}

void intToString( int value )
{
	char arr[16];
	itoa( value, arr, 10);
	LCD_displayString( arr );

}


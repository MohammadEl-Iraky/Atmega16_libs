/*****************************************************************************************************
*  
*  lcd.h
*  Created on: Feb 24, 2020
*  Author: Mohammad Metwally
*
****************************************************************************************************/

#ifndef LCD_H_
#define LCD_H_


/*****************************************************************************************************
*  												Includes
*****************************************************************************************************/

#include "micro_config.h"
#include "std_types.h"

/*****************************************************************************************************
*  											User defined Macros
*****************************************************************************************************/

/* LCD Connection & Configuration */
#define DATA_BITS_MODE				4	/* 8 for 8-bit mode or 4 for 4-bit mode */
#define UPPER_PORT_PINS					/* Comment this depending on your LCD connection */

/* LCD Pin connections */
#define LCD_DATA_PORT				PORTA
#define LCD_DATA_DDR				DDRA
#define LCD_CTRL_PORT				PORTC
#define LCD_CTRL_DDR				DDRC

#define RS							0
#define	RW							1
#define EN							2


/* LCD Commands */
#define CLEAR_SCREEN				0x01
#define TURN_ON_DISPLAY				0x0F
#define FOUR_BITS_MODE				0x02
#define EIGHT_BITS_TWO_LINE			0x38
#define FOUR_BITS_TWO_LINE			0x28

#define CURSOR_CHANGE				0x80

/*****************************************************************************************************
*  										LCD function prototypes
*****************************************************************************************************/

void LCD_init( void );
void LCD_displayCharacter( uint8 data );
void LCD_sendCommand( uint8 command );
void LCD_displayString( const char * str );
void LCD_goToRowCol( uint8 row, uint8 col);
void intToString( int value );



#endif /* LCD_H_ */

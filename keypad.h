/*****************************************************************************************************
*  
*  keypad.h
*  Created on: Feb 24, 2020
*  Author: Mohammad Metwally
*
****************************************************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/*****************************************************************************************************
*  												Includes
*****************************************************************************************************/

#include "micro_config.h"
#include "std_types.h"

/*****************************************************************************************************
*  											User defined Macros
*****************************************************************************************************/

#define ROWS_NO			4
#define COLS_NO			4
#define KEYPAD_PORT		PORTB
#define KEYPAD_DDR		DDRB
#define KEYPAD_PIN		PINB

#define ROWS_OFFSET		0
#define COLS_OFFSET		4


/*****************************************************************************************************
*  										Keypad function prototypes
*****************************************************************************************************/

void KEYPAD_init( void );
uint8 KEYPAD_getPressedKey( void );

#endif /* KEYPAD_H_ */
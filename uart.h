/*****************************************************************************************************
*  
*  uart.h
*  Created on: Jan 19, 2020
*  Author: Mohammad Metwally
*
****************************************************************************************************/

#ifndef UART_H_
#define UART_H_

/*****************************************************************************************************
*  												Includes
*****************************************************************************************************/

#include "std_types.h"
#include "micro_config.h"

/*****************************************************************************************************
*  											UART data types
*****************************************************************************************************/

typedef enum
{
	TXI_DISABLED,
	TXI_ENABLED,
}UART_txInt;

typedef enum
{
	RXI_DISABLED,
	RXI_ENABLED,
}UART_rxInt;

typedef enum
{
	DREI_DISABLED,
	DREI_ENABLED,
}UART_dataEmptyInt;

typedef enum
{
	BITS_5,
	BITS_6,
	BITS_7,
	BITS_8,
	BITS_9 = 7
}UART_data;

typedef enum
{
	NO_PARITY,
	EVEN_PARITY = 2,
	ODD_PARITY = 3
}UART_parity;

typedef enum
{
	ONE_STOP,
	TWO_STOP
}UART_stop;

typedef enum
{
	NORMAL,
	DOUBLE_SPEED
}UART_speedMode;

typedef enum
{
	TX_DISABLED,
	TX_ENABLED
}UART_txEnable;

typedef enum
{
	RX_DISABLED,
	RX_ENABLED
}UART_rxEnable;

typedef struct
{
	unsigned long baud;
	UART_speedMode	mode;
	UART_data data_size;
	UART_parity parity;
	UART_stop stop;
	UART_txEnable tx;
	UART_rxEnable rx;
	UART_txInt txi;
	UART_rxInt rxi;
	UART_dataEmptyInt dri;
}UART_config;

/*****************************************************************************************************
*  										UART Function prototypes
*****************************************************************************************************/

void UART_init( UART_config * config_Ptr );
void UART_sendByte(	uint16 data );
uint8 UART_recieveByte( uint16 * data_Ptr );
void UART_TX_intEnable( void );
void UART_RX_intEnable( void );
void UART_DRE_intEnable(void);
void UART_TX_intDisable( void );
void UART_RX_intDisable( void );
void UART_DRE_intDisable(void);
void UART_TXI_setCallback( void (*Ptr)(void) );
void UART_RXI_setCallback( void (*Ptr)(void) );
void UART_DRE_setCallback( void (*Ptr)(void) );


#endif /* UART_H_ */

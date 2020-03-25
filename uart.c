/*****************************************************************************************************
*  
*  uart.c
*  Created on: Jan 19, 2020
*  Author: Mohammad Metwally
*
****************************************************************************************************/

#include "uart.h"

/*****************************************************************************************************
*  											   Global Variables
*****************************************************************************************************/

static uint8 g_data_size = 0;
static void (* volatile g_txInt_Ptr)(void) = NULL_PTR;
static void (* volatile g_rxInt_Ptr)(void) = NULL_PTR;
static void (* volatile g_dreInt_Ptr)(void) = NULL_PTR;

/*****************************************************************************************************
*  											Function definitions
*****************************************************************************************************/

void UART_init( UART_config * config_Ptr )
{
	uint16 uart_baud = 0;
	/* Place the desired bits size in the global data size variable */
	g_data_size = config_Ptr->data_size;
	/* Select Normal mode or double speed mode by modifying bit 1 ( U2X ) in UCSRA register
	 * 0: Normal speed
	 * 1: Double speed mode
	 */
	UCSRA = ( UCSRA & 0xFD ) | (( config_Ptr->mode )<<U2X);

	/* Select the desired baud rate for Normal/Double speed mode using the following equations
	 *	Normal mode:    UBBR = ( F / 16Baudrate ) - 1
	 *  Double speed:	UBRR = ( F / 8Baudrate ) - 1
	 *
	 */
	switch( config_Ptr->mode )
	{
		case NORMAL:		uart_baud = (( F_OSC / (16*config_Ptr->baud) ) -1 );
							break;
		case DOUBLE_SPEED:	uart_baud = (( F_OSC / (8*config_Ptr->baud) ) -1 );
							break;
	}

	/* Place the value in the UBRR register */
	UBRRL = (uint8) uart_baud;
	UBRRH = (uint8) (uart_baud>>8);

	/* Select the desired parity settings by modifying bits 5,4 ( UPM 1:0 ) in UCSRC register
	 * 00: No parity
	 * 10: Even parity
	 * 11: Odd parity
	 */
	UCSRC = ( 1<<URSEL ) | ( UCSRC & 0xCF ) | (( config_Ptr->parity )<<UPM0);

	/* Select the desired number of stop bits by modifying bit 3 ( USBS ) in UCSRC register
	 * 0: 1 bit
	 * 1: 2 bits
	 */
	UCSRC = ( 1<<URSEL ) | ( UCSRC & 0xF7 ) | (( config_Ptr->stop )<<USBS);

	/* Select the desired data size in bits by modifying bits 1,2 ( UCSZ1:0 ) in UCSRC register
	 * and bit .. in UCSRB register
	 * 000: 5 bits				011: 8 bits
	 * 001: 6 bits				111: 9 bits
	 * 010: 7 bits
	 */
	UCSRC = ( 1<<URSEL ) | ( UCSRC & 0xF9 ) | (( config_Ptr->data_size & 0x03 )<<UCSZ0);
	UCSRB = ( UCSRB & 0xFB ) | (( config_Ptr->data_size & 0x02 )<<UCSZ2);

	/* Enable Tx by modifying bit ( TXEN ) in UCSRB register */
	UCSRB = ( UCSRB & 0xF7 ) | (( config_Ptr->tx )<<TXEN);

	/* Enable Rx by modifying bit ( RXEN ) in UCSRB register */
	UCSRB = ( UCSRB & 0xEF ) | (( config_Ptr->rx)<<RXEN);

	/* Enable Tx interrupt by modifying bit ( TXCIE ) in UCSRB register */
	UCSRB = ( UCSRB & 0xBF ) | (( config_Ptr->txi)<<TXCIE);

	/* Enable Rx interrupt by modifying bit ( RXCIE ) in UCSRB register */
	UCSRB = ( UCSRB & 0x7F ) | (( config_Ptr->rxi)<<RXCIE);

	/* Enable Data register empty interrupt by modifying bit ( UDRIE ) in UCSRB register */
	UCSRB = ( UCSRB & 0xDF ) | (( config_Ptr->dri)<<UDRIE);


}


void UART_sendByte( uint16 data )
{
	/* Wait untill the transmit buffer is empty */
	while( !( UCSRA & ( 1<<UDRE )) );

	/* Send the data depending on whether it's 9 bits or others */
	if( g_data_size == BITS_9 )
	{
		/* If the data is 9 bits, send the 9th bit at bit 0 in UCSRB then send the other 8 bits at UDR register */
		UCSRB |= ( data>>8 );
	}
	else
	{
		/* Empty else */
	}
	UDR = (uint8)data;
}


uint8 UART_recieveByte( uint16 * data_Ptr )
{
	uint8 parity_error = 0;	/* Local variable for holding the parity error status */

	/* Wait untill data is recieved */
	while( !( UCSRA & ( 1<<RXC )) );

	/* If parity is enabled and there's a parity error then set the flag and return the status */
	if( ( UCSRA & ( 1<<PE )) )
	{
		parity_error = 1;
	}
	else
	{
		if( g_data_size == BITS_9 )
		{
			/* If the data is 9 bits, read the MSB first in UCSRB then read the other 8 bits at UDR register */
			*data_Ptr = (( UCSRB & (1<<RXB8) )<<8);
		}
		else
		{
			/* Empty else */
		}
		*data_Ptr |= UDR;
	}
	return parity_error;
}


void UART_TX_intEnable( void )
{
	UCSRB |= ( 1<<TXCIE );
}

void UART_RX_intEnable( void )
{
	UCSRB |= (1<<RXCIE);
}

void UART_DRE_intEnable(void)
{
	UCSRB |= (1<<UDRIE);
}

void UART_TX_intDisable( void )
{
	UCSRB &= ~( 1<<TXCIE );
}

void UART_RX_intDisable( void )
{
	UCSRB &= ~(1<<RXCIE);
}

void UART_DRE_intDisable(void)
{
	UCSRB &= ~(1<<UDRIE);
}

void UART_TXI_setCallback( void (*Ptr)(void) )
{
	g_txInt_Ptr = Ptr;
}

void UART_RXI_setCallback( void (*Ptr)(void) )
{
	g_rxInt_Ptr = Ptr;
}

void UART_DRE_setCallback( void (*Ptr)(void) )
{
	g_dreInt_Ptr = Ptr;
}

ISR(USART_TXC_vect)
{
	if( g_txInt_Ptr != NULL_PTR )
	{
		g_txInt_Ptr();
	}
}

ISR(USART_RXC_vect)
{
	if( g_rxInt_Ptr != NULL_PTR )
	{
		g_rxInt_Ptr();
	}
}

ISR(USART_UDRE_vect)
{
	if( g_dreInt_Ptr != NULL_PTR )
	{
		g_dreInt_Ptr();
	}

}

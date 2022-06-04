#include <stdio.h>
#include "main.h"
#include "usart.h"

extern "C"
{
	void 		Communicate_Initialize(void);
	void 		Communicate_Transmit1byte( uint8_t);
	void 		Communicate_TransmitDMA( uint8_t );
	uint8_t		Communicate_Receive1byte( void );
	uint8_t 	Communicate_ReceiceDMA( void );
	void 		Communicate_ClearReceiveBuffer( void );
}
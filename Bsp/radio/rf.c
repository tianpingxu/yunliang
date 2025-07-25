#include "rf.h"
#include "radio.h"
#include "cmt2300.h"
#include "function.h"

const char *rfName = "CMT2300";

u16	iSend, iRev;	
u8	sendBuf[64];	
u8	revBuf[128];	


void RFInit( void )
{
	RF_Init();
}


void RFRxMode( void )
{
	RF_StartRx(revBuf, 8, INFINITE);
}


u8 RFRevData( u8 *buf )
{
	u8 length = 0;
	switch(RF_Process())
	{
		case RF_IDLE:
		{
			RFRxMode();
			break;
		}
		case RF_RX_DONE:
		{
			length = g_nRxLength;
			RFRxMode();
			if(length>0)iRev++;

		}
		default:
			break;
		}
	return(length);
}


u8 RFSendData( u8 *buf, u8 size )
{
	int ret = 0;

	RF_StartTx(buf, size, 2000);
	
	while (1)
	{
		switch(RF_Process())
		{
			case RF_IDLE:
			case RF_ERROR:
			{				
				RFRxMode();
				//msg( "rf_err" );
				//delay_ms(500);
				return 0;
			}
			case RF_TX_DONE:
			{
				iSend++;
				ret = size;

				RFRxMode();
				
				return ret;
			}
			
			default:			
			{
				//msg( "default" );
				//delay_ms(500);
				break;
			}
		}
			
	}
	
	return(0);
}


#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"
#include "cmt2300.h"
#include "rf.h"
#include "bsp_timer.h"

unsigned short continueSendCnt=0;
unsigned short continueSendIndex=0;

void keyProcess(void)
{
	if(GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_13) == 0x00) {
			vBSP_DelayMs(20);
			if(GPIO_ReadInputDataBit(GPIOC, GPIO_PIN_13) == 0x00) {

			}
	}
}

char rfContinueSend(void)
{
	static int t = 0;
	if(continueSendIndex>=continueSendCnt)
			return 0;
	if (t++ <  500){
		vBSP_DelayMs(1);
	}else {
		continueSendIndex++;
		t = 0;

		RFSendData(sendBuf, 8);

		return 1;
	}
	return 0;		 
}


